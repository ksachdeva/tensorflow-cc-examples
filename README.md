# Tensorflow C++ Examples

This repository contains tensorflow examples written in C++. The examples are primarily geared towards usage of C++ for inference aspect. At present, whereever a model is required I use/provide a pre-trained model and/or a python script to generate the model.

The inspiration for this repository is https://github.com/aymericdamien/TensorFlow-Examples that provides examples written using python.

## Setup

You would need to compile the tensorflow. The instructions are available here -
https://www.tensorflow.org/install/install_sources

Next step is to generate the SDK that can be used as a CMake module. I use another repo (which is a submodule of this repository) of mine to perform this task.

```bash
# Make sure to use the recursive option so that you can get all the dependencies
git clone --recursive https://github.com/ksachdeva/tensorflow-cc-examples
```

```bash
# Generate the tensorflow SDK
# Note - make sure to run the gen_sdk.h from tensorflow-cc-sdk-generator directory
cd <path_to_this_repo>/third-party/tensorflow-cc-sdk-generator
./gen_sdk.sh -r <path_to_tensorflow_clone> -o <path_to_this_repo>/third-party/tensorflow-sdk
```

```bash
# Build all examples using CMake
mkdir build
cd build
cmake ..
cmake --build .
```

You will find the binaries of various examples in the build/bin directory.

## Examples

The examples are heavily commented so there is no separate page to describe them. Below are brief descriptions and order that I believe should be good for a beginner.

### Introduction

* **Hello World** ([code](examples/1_Introduction/src/hello-world.cc)). A very simple example that shows how to create Const Ops that take scalar inputs and use StringJoin operator to output the concatentation result.

* **Naming Ops** ([code](examples/1_Introduction/src/naming-ops.cc)). This simple example shows how to name your ops. This is optional but recommended practice for sophisticated graphs.

* **Basic Operations** ([code](examples/1_Introduction/src/basic-operations.cc)). This example shows base math ops and usage of Placeholders with and without default values when running the session.

* **Basic Tensors** ([code](examples/1_Introduction/src/basic-tensors.cc)). A simple example to construct and inspect various types of tensors.

* **Basic GraphDef** ([code](examples/1_Introduction/src/basic-graphdef.cc)). An example that shows how to use GraphDef and Session api. Also shown is how to use the op names to provide the inputs and extract the outputs from the session run.

* **Basic I/O** ([code](examples/1_Introduction/src/basic-io.cc)). An example to show how to use I/O API in tensorflow to read a file.