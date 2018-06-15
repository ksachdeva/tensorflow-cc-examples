#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/ops/standard_ops.h>
#include <tensorflow/core/framework/tensor.h>

// Basic math operations using tensorflow

// The sample demonstrates how to
// - create various operations
// - use placeholder and placeholderWithDefault
// - use different overloads of Run method

int main(int argc, char **argv) {

  using namespace tensorflow;
  using namespace tensorflow::ops;

  // create a root scope
  auto scope = Scope::NewRootScope();

  // we are creating various local scopes
  // so that a new session object is created
  // for all the examples

  {
    // An example of doing addition
    // on constants

    ClientSession session(scope);

    auto a = Const(scope, 2);
    auto b = Const(scope, 3);

    auto c = Add(scope, a, b);

    std::vector<Tensor> outputs;
    TF_CHECK_OK(session.Run({c}, &outputs));

    // we know that it will be scalar
    // we can also get the underlying data by calling flat
    std::cout << "Underlying Scalar value -> " << outputs[0].flat<int>()
              << std::endl;
  }

  {
    // An example of how to supply a variable (i.e. not a constant)
    // whose value is supplied at the time when we run the session

    ClientSession session(scope);

    // we will use Placeholder as the type for our variables
    auto a = Placeholder(scope, DT_INT32);
    auto b = Placeholder(scope, DT_INT32);

    // define the add operation that takes
    // the placeholders a and b as inputs
    auto c = Add(scope, a, b);

    std::vector<Tensor> outputs;

    // we now specify the values for our placeholders
    // note that the way Run method is called. It is quite
    // different from previous example.
    //
    // Here we are using this overload of Run method
    // Run(const FeedType& inputs, const std::vector<Output>& fetch_outputs,
    //        std::vector<Tensor>* outputs) const;
    //
    //
    // which takes FeedType (alias of std::unordered_map<Output, Input::Initializer, OutputHash>
    // as the first argument.
    // Note - In std::unordered_map OutputHash is optional
    // So we just need to supply a map whose key of type "Output" and the
    // value that respect Initializer
    //
    // {a,2} & {b,3} would satisfiy this requirement since type 'a' & 'b'
    // is Output
    
    auto status = session.Run({
      {
        {a, 2},
        {b, 3}
      } }, {c}, &outputs);
    
    TF_CHECK_OK(status);
    
    // we know that it will be scalar
    // we can also get the underlying data by calling flat
    std::cout << "Underlying Scalar value -> " << outputs[0].flat<int>()
              << std::endl;
  }
  
  {
    // This is yet another example that makes use of Placeholder however
    // this time we want one of the placeholder to have a default value
    //
    // In other words, it does not need to be specified during the session
    // execution. if you give a new value it would accept it else would use
    // the default value
    
    ClientSession session(scope);
    
    // create an input
    auto defaultAInput = Input(8);
    
    // we will use Placeholder as the type for our variables
    auto a = PlaceholderWithDefault(scope, defaultAInput, PartialTensorShape());
    auto b = Placeholder(scope, DT_INT32);
    
    // define the add operation that takes
    // the placeholders a and b as inputs
    auto c = Add(scope, a, b);
    
    std::vector<Tensor> outputs;
    
    // In this Run we are not specifying 'a'
    // so its default value i.e. 8 will be used
    auto status = session.Run({
      {
        {b, 3}
      } }, {c}, &outputs);
    
    TF_CHECK_OK(status);
    
    std::cout << "Underlying Scalar value (using default placeholder value [8]) -> " << outputs[0].flat<int>()
    << std::endl;
    
    // here we do specify a value for placeholder 'a' i.e. 9
    status = session.Run({
      {
        {a, 9},
        {b, 3}
      } }, {c}, &outputs);
    
    TF_CHECK_OK(status);
    
    std::cout << "Underlying Scalar value (after supplying new value [9]) -> " << outputs[0].flat<int>()
    << std::endl;
    
  }

  return 0;
}
