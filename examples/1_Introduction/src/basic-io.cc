#include <tensorflow/core/framework/tensor.h>
#include <tensorflow/core/lib/io/path.h>
#include <tensorflow/core/platform/env.h>

// Tensorflow provides support to perform I/O operations
// as well. Normally you should be able to perform these types
// of operations using the library/framework of your choice.
// I believe reason tensorflow provides the support is to provide
// an abstraction layer for various platforms it supports.
//
// In this example we are going to look at few I/O related
// apis.
//
// Run this example as:
// <path_to>/bin/intro-basic-io <path_to>/data/hello.txt

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Provide the path to the input file .." << std::endl;
    std::cout << "Usage: intro-basic-io data/hello.txt" << std::endl;
    return -1;
  }

  auto filePath = std::string(argv[1]);

  // The first thing to do is to get a handle on the underlying
  // platform / OS on which this app is running.
  auto env = tensorflow::Env::Default();

  // check if a file provided as the input argument exists or not
  auto status = env->FileExists(filePath);
  if (status.ok() == false) {
    std::cerr << "Invalid path to the file .." << std::endl;
    return -1;
  }

  // get the size of the file
  tensorflow::uint64 file_size;
  status = env->GetFileSize(filePath, &file_size);

  if (status.ok() == false) {
    std::cerr << "Unable to get the size of the file .." << std::endl;
    return -1;
  }

  std::cout << "File size is - " << file_size << std::endl;

  // In order to read the content of the file
  // we first get a file handle

  std::unique_ptr<tensorflow::RandomAccessFile> theFile;
  status = env->NewRandomAccessFile(filePath, &theFile);

  if (status.ok() == false) {
    std::cerr << "Unable to open the file .." << std::endl;
    return -1;
  }

  // now using this handle we read it out
  //
  // it would be required to specify from which offset in the file
  // we want to read and how many bytes we want to read
  //
  // it should be evident that trying to read more bytes than the ones
  // in the file will lead to error very much like it happens in any I/O
  // based API. In other words, it is up to the caller to make sure that the
  // arguments of Read api are sane.
  //
  // Read api takes 4 arguments - offset, number of bytes to read, StringPiece
  // and pointer to address where the api will store the result
  //

  // allocate the data where we will store the output of the Read api
  std::string content;
  content.resize(file_size);

  // create StringPiece which is nothing but a view of any external data
  // location
  //
  // Note that it is the API that is setting StringPiece to the content
  tensorflow::StringPiece sp;

  status = theFile->Read(0, file_size, &sp, &(content)[0]);

  if (status.ok() == false) {
    std::cerr << "Failed to read the file .." << std::endl;
    return -1;
  }

  std::cout << content << std::endl;

  // You can now get the data by StringPiece or
  // the original location and convert it into the tensor
  // (in this particular example it would be Scalar)
  tensorflow::Tensor result(tensorflow::DT_STRING, tensorflow::TensorShape());
  result.scalar<std::string>()() = sp.ToString();

  // As should be clear from this example one could easily use the
  // I/O api that are not necessarily part of tensorflow

  return 0;
}
