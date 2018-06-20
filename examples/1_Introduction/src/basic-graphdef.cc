#include <tensorflow/cc/ops/standard_ops.h>
#include <tensorflow/core/framework/tensor.h>
#include <tensorflow/core/public/session.h>

// So far in other examples we have executed the session
// using the scope.
//
// There is an another way of performing the same operation and that is
// by creating the GraphDef object. This method is interesting as with this
// one you can work with op names for both inputs and outputs. Also it is
// the one that is required to run a model that is stored in a file.
//
// Reality is that in the previous example there is already a graph. The
// default graph. It is being implicitly passed to all APIs in the same
// context (scope).

int main(int argc, char** argv) {
  using namespace tensorflow;
  using namespace tensorflow::ops;

  {
    // create a root scope
    auto scope = Scope::NewRootScope();

    auto a = Const(scope.WithOpName("A"), 23);
    auto b = Const(scope.WithOpName("B"), 2);

    // here we are creating two ops (Add & Sub)
    // Add - a + b
    // Sub - Result of (a+b) - b
    //
    // Here you can see that Sub requires Add operation
    auto add = Add(scope.WithOpName("Add"), a, b);
    auto sub = Sub(scope.WithOpName("Sub"), add, b);

    // essentially we are are getting the default graph
    // that gets created by the framework for the root scope
    GraphDef graph;
    TF_CHECK_OK(scope.ToGraphDef(&graph));

    // so far in other examples we have used ClientSession
    // here is another way to create the session object.
    //
    // While ClientSession is used to execute graph created with in
    // the C++ code, it is desirable (most of the time) to load the
    // serialized graph.
    //
    // Note that in this example we are still creating the grpah in C++. In
    // other words not loading from a file.
    std::unique_ptr<tensorflow::Session> session(
        tensorflow::NewSession(tensorflow::SessionOptions()));

    // we are binding the session to this graph
    TF_CHECK_OK(session->Create(graph));

    std::vector<Tensor> outputs;

    // This is where you will see differences between ClientSession & Session
    // more clearly
    //
    // The Run method of Session is quite different from that of ClientSession
    //
    // Here instead of providing the final node of the constructed graph as the
    // input we would supply the Tensor values as the input. To be more clear,
    // a vector of pairs that provide the names of the inputs and their
    // corresponing value. That said, in this particular example the inputs (A &
    // B) are constants so we do not need to specify them.
    //
    // This is why the first argument of Run is {}
    // The second argument is the names of the output nodes that you want to
    // extract

    TF_CHECK_OK(session->Run({}, {"Sub"}, {}, &outputs));

    std::cout << "Size of outputs -> " << outputs.size() << std::endl;
    std::cout << "DebugString Sub -> " << outputs[0].DebugString() << std::endl;

    //
    // What if we wanted the outputs of Add & Sub. Note that Add is not
    // necessarily the last node in our graph but it is still possible to
    // extract the value of A node. Simply specify the names of the output nodes
    // that you want

    outputs.clear();

    TF_CHECK_OK(session->Run({}, {"Add", "Sub"}, {}, &outputs));

    std::cout << "Size of outputs -> " << outputs.size() << std::endl;
    std::cout << "DebugString Add -> " << outputs[0].DebugString() << std::endl;
    std::cout << "DebugString Sub -> " << outputs[1].DebugString() << std::endl;
  }

  {
    // this is very similar example as above but in this case
    // we intend to also supply the values of the inputs i.e. not use
    // Consts but Placeholders

    auto scope = Scope::NewRootScope();

    auto a = Placeholder(scope.WithOpName("A"), DT_INT32);
    auto b = Placeholder(scope.WithOpName("B"), DT_INT32);

    auto add = Add(scope.WithOpName("Add"), a, b);

    GraphDef graph;
    TF_CHECK_OK(scope.ToGraphDef(&graph));

    std::unique_ptr<tensorflow::Session> session(
        tensorflow::NewSession(tensorflow::SessionOptions()));

    // we are binding the session to this graph
    TF_CHECK_OK(session->Create(graph));

    std::vector<Tensor> outputs;

    // create two tensors
    auto inputA = Tensor(DT_INT32, TensorShape({}));
    auto inputB = Tensor(DT_INT32, TensorShape({}));

    // set their values
    inputA.scalar<int>()() = 10;
    inputB.scalar<int>()() = 20;

    // This is where you would want to pay attention to
    // Here we are specifying the inputs using the name
    // of the nodes (see how Placeholders were created)
    std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
        {"A", inputA},
        {"B", inputB},
    };

    TF_CHECK_OK(session->Run(inputs, {"Add"}, {}, &outputs));

    std::cout << "Size of outputs -> " << outputs.size() << std::endl;
    std::cout << "DebugString Add -> " << outputs[0].DebugString() << std::endl;
  }

  return 0;
}