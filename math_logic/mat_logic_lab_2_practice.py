from sympy import symbols, And, Or, Not, Implies
from sympy.logic.boolalg import simplify_logic
from sympy.parsing.sympy_parser import parse_expr
from itertools import product
from colorama import Fore, Back, Style, init

class ImpliesLab(Implies):
    def __str__(self):
        return f"{self.args[0]} -> {self.args[1]}"

class OrLab(Or):
    def __str__(self):
        return f"{self.args[0]} or {self.args[1]}"

class AndLab(And):
    def __str__(self):
        return f"{self.args[0]} and {self.args[1]}"

class NotLab(Not):
    def __str__(self):
        return f"not {self.args[0]}"

def extract_variables(formula):
    variables = set()
    if formula.is_Atom:
        variables.add(formula)
    elif formula.args:
        for arg in formula.args:
            variables.update(extract_variables(arg))
    return variables

def generate_simple_formulas(variables):
    variables_set = set(variables)
    formulas = set(variables_set | {NotLab(v) for v in variables_set})

    for v1, v2 in product(formulas, repeat=2):
        formulas.add(OrLab(v1, v2))
        formulas.add(AndLab(v1, v2))
        formulas.add(ImpliesLab(v1, v2))

    return formulas

def is_consequence(input_formula, candidate_formula, variables):
    for values in product([True, False], repeat=len(variables)):
        input_truth = input_formula.subs(dict(zip(variables, values)))
        candidate_truth = candidate_formula.subs(dict(zip(variables, values)))

        if input_truth and not candidate_truth:
            return False
    return True

def is_nontrivial_formula(formula):
    return not (formula.is_Atom or formula.is_Not and formula.args[0].is_Atom)

def print_formula(formula):
    return str(formula)

def main():
    # Инициализация colorama
    init(autoreset=True)

    print(Fore.GREEN + Style.BRIGHT + "="*50)
    print(Fore.GREEN + Style.BRIGHT + "Логический анализатор формул")
    print(Fore.GREEN + "="*50)

    print(Fore.CYAN + "\nВыберите форму:")
    print(Fore.CYAN + "1. Конъюнктивная нормальная форма (КНФ)")
    print(Fore.CYAN + "2. Дизъюнктивная нормальная форма (ДНФ)")
    
    choice = input(Fore.YELLOW + Style.BRIGHT + "\nВаш выбор (1/2): ")

    if choice not in {"1", "2"}:
        print(Fore.RED + Style.BRIGHT + "\nНеверный выбор. Попробуйте снова.")
        return

    print(Fore.GREEN + "\nВведите логическую формулу в стандартной записи.")
    print("Используйте операторы: (and), (or), (not), (->) импликация запрещена.")
    input_formula = input(Fore.YELLOW + Style.BRIGHT)

    input_formula = input_formula.replace("and", " & ").replace("or", " | ").replace("not", " ~ ").replace("<=", ">>")

    # Разбираем формулу
    input_formula_parsed = parse_expr(input_formula, evaluate=False)

    # Извлекаем переменные
    variables = list(extract_variables(input_formula_parsed))

    # Проверка нормальной формы
    if choice == "1":  # Проверяем КНФ
        simplified = simplify_logic(input_formula_parsed, form="cnf")
        print(Fore.GREEN + "\n" + "="*50)
        if input_formula_parsed == simplified:
            print(Fore.GREEN + Style.BRIGHT + "Формула корректна КНФ.")
        else:
            print(Fore.RED + Style.BRIGHT + "Формула не является КНФ. Она будет преобразована в КНФ:")
            print(Fore.CYAN + f"  {simplified}")
            input_formula_parsed = simplified
        print(Fore.GREEN + "="*50)
    elif choice == "2":  # Проверяем ДНФ
        simplified = simplify_logic(input_formula_parsed, form="dnf")
        print(Fore.GREEN + "\n" + "="*50)
        if input_formula_parsed == simplified:
            print(Fore.GREEN + Style.BRIGHT + "Формула корректна в ДНФ.")
        else:
            print(Fore.RED + Style.BRIGHT + "Формула не является ДНФ. Она будет преобразована в ДНФ:")
            print(Fore.CYAN + f"  {simplified}")
            input_formula_parsed = simplified
        print(Fore.GREEN + "="*50)

    # Генерация простых формул
    candidate_formulas = generate_simple_formulas(variables)

    # Выводим следствия
    consequences = [
        f for f in candidate_formulas
        if is_consequence(input_formula_parsed, f, variables) and f != input_formula_parsed and is_nontrivial_formula(f)
    ]

    if consequences:
        print(Fore.GREEN + "\n" + "="*50)
        print(Fore.GREEN + Style.BRIGHT + "Следствия:")
        for consequence in consequences:
            print(Fore.CYAN + f"  {print_formula(consequence)}")
        print(Fore.GREEN + "="*50)
    else:
        print(Fore.RED + "\n" + "="*50)
        print(Fore.RED + Style.BRIGHT + "Нет следствий из данной формулы.")
        print(Fore.RED + "="*50)

if __name__ == "__main__":
    main()
