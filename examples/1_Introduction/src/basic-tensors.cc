#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/ops/standard_ops.h>
#include <tensorflow/core/framework/tensor.h>

// Basic tensor operations using tensorflow

// The sample demonstrates how to
// - create various operations
// - use placeholder and placeholderWithDefault
// - use different overloads of Run method

int main(int argc, char **argv) {
  using namespace tensorflow;
  using namespace tensorflow::ops;

  // create a root scope
  auto scope = Scope::NewRootScope();

  {
    // we already know how to create Scalar
    // which is a tensor of Rank 0
    auto aScalar = Input(2);
    std::cout << "Dimensions of a scalar - " << aScalar.tensor().shape().dims()
              << std::endl;

    // A tensor of Rank 1 is called a vector
    auto aVector = Input({2, 3});
    std::cout << "Dimensions of a vector - " << aVector.tensor().shape().dims()
              << std::endl;

    // A tensor of Rank 2 is called a matrix
    auto aMatrix = Input({{2, 3}, {6, 5}});
    std::cout << "Dimensions of a matrix - " << aMatrix.tensor().shape().dims()
              << std::endl;

    // A tensor of Rank 3 or more is not known by any special name tensor
  }

  {
    // When building the ops you can specify the shape explicitly as well

    // 2x2 matrix with all elements = 10
    auto c1 = Const(scope, 10, /* shape */ {2, 2});

    // [1 1] * [41; 1]
    auto x = MatMul(scope, {{1, 1}}, {{41}, {1}});

    ClientSession session(scope);

    std::vector<Tensor> outputs;

    auto status = session.Run({x}, &outputs);

    TF_CHECK_OK(status);

    std::cout << "Underlying Scalar value -> " << outputs[0].flat<int>()
              << std::endl;

    // [1 2 3 4] + 10
    auto y = Add(scope, {1, 2, 3, 4}, 10);

    status = session.Run({y}, &outputs);

    TF_CHECK_OK(status);

    std::cout << "Underlying vector value -> " << outputs[0].flat<int>()
              << std::endl;
  }

  return 0;
}
