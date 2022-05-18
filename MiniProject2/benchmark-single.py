from __future__ import absolute_import, division, print_function, unicode_literals

import tensorflow_datasets as tfds
import tensorflow as tf
tfds.disable_progress_bar()

import time
import os
import json

from tensorflow.python.client import device_lib
print(device_lib.list_local_devices())
print("Num GPUs Available: ", len(tf.config.experimental.list_physical_devices('GPU')))

def get_dataset(N, dataset='mnist'):
    '''
    Download the MNIST dataset and load it from [TensorFlow Datasets]
    (https://www.tensorflow.org/datasets). This returns a dataset in `tf.data` format.
    The helper function `get_dataset` also handles repeating the dataset N times
    when we want to scale it up for the experiment.
    '''
    d = tfds.load(name=dataset, as_supervised=True)
    mnist_train, mnist_test = d['train'], d['test']

    for i in range(1, N):
        d = tfds.load(name=dataset, as_supervised=True)
        single_mnist_train, single_mnist_test = d['train'], d['test']

        mnist_train = mnist_train.concatenate(single_mnist_train)
        mnist_test = mnist_test.concatenate(single_mnist_test)

    return mnist_train, mnist_test

def scale(image, label):
    '''
    Pixel values, which are 0-255, [have to be normalized to the 0-1 range]
    (https://en.wikipedia.org/wiki/Feature_scaling). Define this scale in a function.
    '''
    image = tf.cast(image, tf.float32)
    image /= 255

    return image, label

def define_small_model():
    model = tf.keras.Sequential([
      tf.keras.layers.Conv2D(32, 3, activation='relu', input_shape=(28, 28, 1)),
      tf.keras.layers.MaxPooling2D(),
      tf.keras.layers.Flatten(),
      tf.keras.layers.Dense(64, activation='relu'),
      tf.keras.layers.Dense(10, activation='softmax')
    ])

    model.compile(loss='sparse_categorical_crossentropy',
                optimizer=tf.keras.optimizers.Adam(),
                metrics=['accuracy'])
    return model

def define_large_model():
    model = tf.keras.Sequential([
        tf.keras.layers.Conv2D(128, 3, activation='relu', input_shape=(28, 28, 1)),
        tf.keras.layers.MaxPooling2D(),
        tf.keras.layers.Conv2D(256, 3, activation='relu'),
        tf.keras.layers.MaxPooling2D(),
        tf.keras.layers.Conv2D(512, 3, activation='relu'),
        tf.keras.layers.MaxPooling2D(),
        tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(512, activation='relu'),
        tf.keras.layers.Dense(512, activation='relu'),
        tf.keras.layers.Dense(10, activation='softmax')
        ])

    model.compile(loss='sparse_categorical_crossentropy',
                optimizer=tf.keras.optimizers.Adam(),
                metrics=['accuracy'])
    return model

def define_xl_model():
    model = tf.keras.Sequential([
        tf.keras.layers.Conv2D(128, 3, activation='relu', input_shape=(28, 28, 1)),
        tf.keras.layers.MaxPooling2D(),
        tf.keras.layers.Conv2D(256, 3, activation='relu'),
        tf.keras.layers.MaxPooling2D(),
        tf.keras.layers.Conv2D(512, 3, activation='relu'),
        tf.keras.layers.MaxPooling2D(),
        tf.keras.layers.Conv2D(1024, 3, activation='relu'),
        tf.keras.layers.MaxPool2D(),
        tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(512, activation='relu'),
        tf.keras.layers.Dense(512, activation='relu'),
        tf.keras.layers.Dense(10, activation='softmax')
        ])

    model.compile(loss='sparse_categorical_crossentropy',
                optimizer=tf.keras.optimizers.Adam(),
                metrics=['accuracy'])
    return model

def decay(epoch):
    '''
    Function for decaying the learning rate.
    You can define any decay function you need.
    '''
    if epoch < 3:
        return 1e-3
    elif epoch >= 3 and epoch < 7:
        return 1e-4
    else:
        return 1e-5

def experiment(define_model_fnc, n_dataset_repeat, batch_size_per_replica, n_gpus, record_results=False):

    # define data parallel strategy if using more than one gpu
    #if n_gpus > 1:
    #    strategy = tf.distribute.MirroredStrategy(devices=["/gpu:{}".format(n_gpus)])
    #    print('Number of devices: {}'.format(strategy.num_replicas_in_sync))

    # set up batch size var. depends on how many gpus are being used
    BUFFER_SIZE = 10000

    #if n_gpus > 1:
    #    BATCH_SIZE = batch_size_per_replica * strategy.num_replicas_in_sync
    #else:
    BATCH_SIZE = batch_size_per_replica

    # download and process dataset
    mnist_train, mnist_test = get_dataset(n_dataset_repeat, 'mnist')
    train_dataset = mnist_train.map(scale).cache().shuffle(BUFFER_SIZE).batch(BATCH_SIZE)
    eval_dataset = mnist_test.map(scale).batch(BATCH_SIZE)

    # Create and compile the Keras model in the context of `strategy.scope`.
    #if n_gpus>1:
    #    with strategy.scope():
    #        model = define_model_fnc()
    #else:
    #    model = define_model_fnc()

    strategy = tf.distribute.MirroredStrategy(['GPU:' + str(n_gpus)])
    with strategy.scope():
        model = define_model_fnc()

    # define callback that will record the epoch train time
    epoch_times = []
    class timecallback(tf.keras.callbacks.Callback):
        def on_epoch_begin(self,epoch,logs={}):
            self.starttime = time.clock()

        def on_epoch_end(self,epoch,logs = {}):
            epoch_times.append(time.clock() - self.starttime)

    callbacks = [
        tf.keras.callbacks.TensorBoard(log_dir='./logs'),
        tf.keras.callbacks.LearningRateScheduler(decay),
        timecallback(),
    ]

    model.fit(train_dataset, epochs=10, callbacks=callbacks)

    if record_results:
        with open('data/tensorflow_results_avg.txt', 'a') as f:
            f.write(json.dumps(
                        {'n_dataset_repeat': n_dataset_repeat,
                        'batch_size': batch_size_per_replica,
                        'n_gpus': n_gpus,
                        'first epoch time': epoch_times[0],
                        'avg epoch time': sum(epoch_times[1:]) / (len(epoch_times) - 1)}) + '\n')

MODELS = [define_large_model]
DATASET_REPEATS = [4]
BATCH_SIZES = [128, 256, 512]
GPU_NUMS = [1,0,2,3]

for d in DATASET_REPEATS:
    for b in BATCH_SIZES:
        for g in GPU_NUMS:
            for m in MODELS:
                print('\n' + '*'*80 + '\n')
                print('Now training: {} on dataset repeated {}x with batch size {} on {} gpu(s)'.format(m.__name__, d, b, g))
                experiment(m,d,b,g, record_results=True)
