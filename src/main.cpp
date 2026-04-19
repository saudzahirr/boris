#include <iostream>
#include <Eigen/Sparse>
#include <Eigen/Dense>

int main()
{
    const int n = 10;

    Eigen::SparseMatrix<double> A(n, n);
    Eigen::VectorXd b(n);
    Eigen::VectorXd p(n);

    std::vector<Eigen::Triplet<double>> T;

    double K = 1.0;

    for (int i = 0; i < n; i++)
    {
        b(i) = 1.0;

        // 🔴 Boundary conditions (IMPORTANT FIX)
        if (i == 0)
        {
            T.emplace_back(i, i, 1.0);
            b(i) = 100.0;   // left pressure fixed
        }
        else if (i == n - 1)
        {
            T.emplace_back(i, i, 1.0);
            b(i) = 50.0;    // right pressure fixed
        }
        else
        {
            double diag = 0.0;

            T.emplace_back(i, i - 1, -K);
            diag += K;

            T.emplace_back(i, i + 1, -K);
            diag += K;

            T.emplace_back(i, i, diag);
        }
    }

    A.setFromTriplets(T.begin(), T.end());

    // Solve
    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.compute(A);

    if (solver.info() != Eigen::Success)
    {
        std::cout << "Decomposition failed\n";
        return -1;
    }

    p = solver.solve(b);

    if (solver.info() != Eigen::Success)
    {
        std::cout << "Solve failed\n";
        return -1;
    }

    std::cout << "Solution:\n";
    std::cout << p << "\n";

    return 0;
}