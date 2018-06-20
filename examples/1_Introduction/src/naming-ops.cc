#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/ops/standard_ops.h>

// Naming operations

// The sample demonstrates how to
// - create various ops that have a friendly name

int main(int argc, char** argv) {
  using namespace tensorflow;
  using namespace tensorflow::ops;

  // create a root scope
  auto scope = Scope::NewRootScope();

  // here we have create two constants with the names
  auto a = Const(scope.WithOpName("A"), 23);
  auto b = Const(scope.WithOpName("B"), 2);

  // here we are creating two ops (Add & Sub)
  // Add - a + b
  // Sub - Result of (a+b) - b
  //
  // Here you can see that Sub requires Add operation
  auto add = Add(scope.WithOpName("Add"), a, b);
  auto sub = Sub(scope.WithOpName("Sub"), add, b);

  ClientSession session(scope);

  // Run
  std::vector<Tensor> outputs;

  // Now we are interested in getting the output from
  // both Add & Sub (recall that the Sub depends on Add)
  //
  // In order to get the output for both ops we have to specify
  // both of them as inputs
  //
  // Note - if you specify only "sub" then you would get
  // only 1 output i.e. result of sub operation
  TF_CHECK_OK(session.Run({add, sub}, &outputs));

  // See our output using DebugString that tells
  // more information about the tensor
  std::cout << "DebugString Add -> " << outputs[0].DebugString() << std::endl;
  std::cout << "DebugString Sub -> " << outputs[1].DebugString() << std::endl;

  outputs.clear();

  // Does the order of how we specify the inputs matter ?
  //
  // it does not because these ops are just nodes and
  // are evaulated lazily. Since sub depends on add even though in the
  // below example we are specifying sub as the first input it would be
  // evaluated only after add
  TF_CHECK_OK(session.Run({sub, add}, &outputs));

  std::cout << "DebugString Sub -> " << outputs[0].DebugString() << std::endl;
  std::cout << "DebugString Add -> " << outputs[1].DebugString() << std::endl;

  return 0;
}