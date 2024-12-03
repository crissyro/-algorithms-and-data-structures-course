from sympy import symbols, Or, And, Not
from sympy.logic.boolalg import simplify_logic
from sympy.parsing.sympy_parser import parse_expr
from colorama import Fore, Style, init

def parse_input_formula(input_str):
    """Парсит формулу из строки и возвращает выражение."""
    input_str = input_str.replace("and", "&").replace("or", "|").replace("not", "~").replace("->", ">>")
    return parse_expr(input_str, evaluate=False)

def convert_to_cnf(formula):
    """Преобразует формулу в КНФ."""
    return simplify_logic(formula, form="cnf")

def extract_clauses(cnf_formula):
    """Извлекает дизъюнкты из формулы в КНФ."""
    if cnf_formula.func == Or:  # Формула — один дизъюнкт
        return {frozenset(cnf_formula.args)}
    elif cnf_formula.func == And:  # Формула — конъюнкция дизъюнктов
        return {frozenset(arg.args) if arg.func == Or else frozenset([arg]) for arg in cnf_formula.args}
    else:  # Формула — одиночный литерал
        return {frozenset([cnf_formula])}

def resolve(clause1, clause2):
    """Выполняет резолюцию между двумя дизъюнктами."""
    resolvents = set()
    for literal in clause1:
        if Not(literal) in clause2 or Not(literal).simplify() in clause2:
            new_clause = (clause1 | clause2) - {literal, Not(literal)}
            resolvents.add(frozenset(new_clause))
    return resolvents

def resolution(propositional_clauses, goal_clauses):
    """Метод резолюции для доказательства теоремы."""
    clauses = propositional_clauses | goal_clauses
    new = set()

    explanation = []

    while True:
        pairs = [(ci, cj) for ci in clauses for cj in clauses if ci != cj]
        found_empty_clause = False

        for (ci, cj) in pairs:
            resolvents = resolve(ci, cj)
            explanation.append((ci, cj, resolvents))
            if frozenset() in resolvents:
                found_empty_clause = True
                break
            new.update(resolvents)

        if found_empty_clause:
            return True, explanation  

        if new.issubset(clauses):
            return False, explanation  

        clauses.update(new)

def print_explanation(explanation, is_proven):
    """Выводит шаги выполнения резолюции."""
    print(Fore.GREEN + "\nШаги выполнения:")
    for step, (clause1, clause2, resolvents) in enumerate(explanation, start=1):
        print(Fore.CYAN + f"Шаг {step}:")
        print(Fore.YELLOW + f"  Склеиваем: {set(clause1)} и {set(clause2)}")
        if frozenset() in resolvents:
            print(Fore.RED + f"  Результат: {resolvents} (пустое множество, доказательство завершено)")
            break
        else:
            print(Fore.YELLOW + f"  Результат: {resolvents}")

    if not is_proven:
        print(Fore.RED + "Рассуждения завершены: пустая резольвента не найдена. Следствие не доказано.")

def main():
    init(autoreset=True)

    print(Fore.GREEN + Style.BRIGHT + "=" * 50)
    print(Fore.GREEN + Style.BRIGHT + "Метод резолюции для логики высказываний")
    print(Fore.GREEN + "=" * 50)

    num_premises = int(input(Fore.YELLOW + "Введите количество посылок: "))
    premises = []
    for i in range(num_premises):
        formula_str = input(Fore.CYAN + f"Введите посылку {i + 1}: ")
        premises.append(parse_input_formula(formula_str))

    goal_str = input(Fore.CYAN + "Введите следствие: ")
    goal_formula = parse_input_formula(goal_str)

    premises_cnf = [convert_to_cnf(p) for p in premises]
    goal_cnf = Not(goal_formula).simplify()
    goal_cnf = convert_to_cnf(goal_cnf)

    propositional_clauses = set()
    for premise_cnf in premises_cnf:
        propositional_clauses.update(extract_clauses(premise_cnf))

    goal_clauses = extract_clauses(goal_cnf)

    print(Fore.GREEN + "\nПосылки в КНФ:")
    for clause in propositional_clauses:
        print(Fore.YELLOW + f"  {set(clause)}")

    print(Fore.GREEN + "\nОтрицание следствия в КНФ:")
    for clause in goal_clauses:
        print(Fore.YELLOW + f"  {set(clause)}")

    is_proven, explanation = resolution(propositional_clauses, goal_clauses)

    print(Fore.GREEN + "\n" + "=" * 50)
    if is_proven:
        print(Fore.GREEN + Style.BRIGHT + "Теорема доказана (найдена пустая резольвента).")
    else:
        print(Fore.RED + Style.BRIGHT + "Теорема опровергнута (пустая резольвента не найдена).")
    print(Fore.GREEN + "=" * 50)

    print_explanation(explanation, is_proven)

if __name__ == "__main__":
    main()
