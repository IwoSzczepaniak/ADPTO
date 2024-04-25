from typing import Dict, List, Optional, Tuple, Union


_recurrence_counter = 0


def _simplify_clause(clause: List[int], values: Dict[int, int]) -> Optional[List[int]]:
    """
    Simplifies a single SAT formula clause (an alternative of variables), i.e.
    removes False variables and returns satisfied formula if any variable is True.

    For clauses we assume that:
    - None clause is satisfied (since it's a lack of clause)
    - [] clause is not satisfied (since it's an empty alternative, similar to
      sum of zeroes being zero)

    :param clause: clause, list of integers representing variables
    :param values: values of variables
    :return: one of:
    - None if clause is satisfied
    - simplified clause otherwise
    """
    simplified_clause = []
    for variable in clause:
      if variable in values:
        if values[variable] == 1:
          return None  # clause is satisfied
      else:
        simplified_clause.append(variable)
    return simplified_clause




def _simplify_formula(
    formula: List[List[int]], values: Dict[int, int]
) -> Optional[List[List[int]]]:
    """
    Simplifies a SAT formula, i.e. simplifies each clause in the formula.

    For formulas we assume that:
    - None formula is not satisfied
    - [] formula is satisfied (since it's an empty conjunction, similar to
      product of ones being one)

    :param formula: list of clauses in CNF form
    :param values: values of variables
    :return: simplified formula
    """
    simplified_formula = []
    for clause in formula:
      simplified_clause = _simplify_clause(clause, values)
      if simplified_clause is None:
        continue
      if simplified_clause is []:
        return None  # formula is not satisfied
      simplified_formula.append(simplified_clause)
    return simplified_formula


def _solve(
    formula: List[List[int]], values: Dict[int, int]
) -> Union[Dict[int, int], str]:
    """
    Helper function for DPLL solver.

    For clauses we assume that:
    - None clause is satisfied (since it's a lack of clause)
    - [] clause is not satisfied (since it's an empty alternative, similar to
      sum of zeroes being zero)

    For formulas we assume that:
    - None formula is not satisfied
    - [] formula is satisfied (since it's an empty conjunction, similar to
      product of ones being one)

    :param formula: list of clauses in CNF form
    :param values: dictionary of values of clause variables, mapping variable
    number (starting from 1) to either -1 (False) or 1 (True)
    :return: values
    """
    global _recurrence_counter
    _recurrence_counter += 1

    if _simplify_formula(formula, values) is None:
      return values
    
    v = len(values)

    new_values = values.copy()
    new_values[v] = 1
    new_values[-v] = -1
    new_formula = _simplify_formula(formula, new_values)

    if _solve(new_formula, new_values):
      return new_values
    
    new_values = values.copy()
    new_values[v] = -1
    new_values[-v] = 1
    new_formula = _simplify_formula(formula, new_values)

    if _solve(new_formula, new_values):
      return new_values    

    return "UNSAT"

    

def basic_dpll_solve(
    formula: List[List[int]],
) -> Tuple[Union[Dict[int, int], str], int]:
    """
    Basic DPLL (Davis, Putnam, Logemann, Loveland) solver implementation for
    checking SAT formulas satisfiability.

    :param formula: list of clauses in CNF form
    :returns: tuple of:
    - list of values if formula is satisfiable, "UNSAT" otherwise
    - number of recurrent calls to the solver procedure
    """
    global _recurrence_counter
    _recurrence_counter = 0

    result = _solve(formula, values=dict())
    return result, _recurrence_counter
