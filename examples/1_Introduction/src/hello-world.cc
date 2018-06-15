#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/ops/standard_ops.h>
#include <tensorflow/core/framework/tensor.h>

// Simple hello world using TensorFlow

// The sample demonstrates how to
// - create various ops (Const & StringJoin)
// - pass them around for e.g. StringJoin takes an list of other ops
// - pass the final ops to the session
// - get the result of the session
// - a simple peek inside the output using the DebugString & by flattening it

int main(int argc, char **argv) {
  using namespace tensorflow;
  using namespace tensorflow::ops;

  // create a root scope
  auto scope = Scope::NewRootScope();

  // define various constans/inputs on which we
  // will perform an operation
  auto hello = Const(scope, std::string("hello"));
  auto space = Const(scope, std::string(" "));
  auto world = Const(scope, std::string("world !"));

  // StringJoin operation
  auto joinOp = StringJoin(scope, {hello, space, world});

  // create a session that takes our
  // scope as the root scope
  ClientSession session(scope);

  // Run
  std::vector<Tensor> outputs;
  TF_CHECK_OK(session.Run({joinOp}, &outputs));

  // See our output using DebugString that tells
  // more information about the tensor
  std::cout << "DebugString -> " << outputs[0].DebugString() << std::endl;

  // we can also get the underlying data by calling flat
  std::cout << "Underlying Scalar value -> " << outputs[0].flat<std::string>()
            << std::endl;

  return 0;
}