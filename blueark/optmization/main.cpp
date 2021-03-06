#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <vector>
#include <fstream>
#include <string>
#include <fstream>


typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;
using std::cin;
using std::cout;


int main(int argc, char *argv[])
{

  std::ifstream in_file;

  in_file.open(argv[1]);

  int n, m;

  in_file >> n >> m;
  std::vector<std::string> names(n);
  Program lp(CGAL::SMALLER, true, 0, false, 0);
  double a, b, c, eq;

  for (int var = 0; var < n; ++var)
  {
    // a <= var <= b
    in_file >> a >> b >> names[var] >> c;

    lp.set_c(var, -c);
    if (a > 0)
    {
      lp.set_l(var, true, a);
    }
    if (b >= 0)
    {
      lp.set_u(var, true, b);
    }
  }

  in_file.close();
  // each of t)he following m lines is:
  // a[i][0] ... a[i][n-1] <= b[i]
  // where "<=" is an integer, 0 if equal

  in_file.open(argv[2]);
  for (int constraint = 0; constraint < m; ++constraint)
  {
    for (int variable = 0; variable < n; ++variable)
    {

      in_file >> a;

      lp.set_a(variable, constraint, a);
    }
    in_file >> eq >> b;
    lp.set_b(constraint, b);

    if (eq == 1)
    {
      lp.set_r(constraint, CGAL::SMALLER);
    }
  }

  in_file.close();
  std::ofstream file;
  file.open(argv[3], std::ofstream::out | std::ofstream::trunc);

  Solution s = CGAL::solve_linear_program(lp, ET());
  if (!s.solves_linear_program(lp) || s.status() == CGAL::QP_INFEASIBLE)
    file << "no\n";
  else
  {

    file << -CGAL::to_double(s.objective_value()) << "\n";
  }
  CGAL::Quadratic_program_solution<ET>::Variable_value_iterator
      opt = s.variable_values_begin();
  for (int i = 0; i < n; ++i)
  {
    file << names[i] << "," << CGAL::to_double(*(opt + i)) << "\n";
  }
  file.close();

  return 0;
}
