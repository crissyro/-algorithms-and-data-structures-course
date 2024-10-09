import tkinter as tk
from itertools import product
import re


class LogicSolver:
    """
    @brief Класс для решения задач по логическим формулам в КНФ.
    Этот класс предоставляет возможности для:
    - Проверки противоречивости формулы.
    - Нахождения всех истинных интерпретаций.
    - Вывода таблицы истинности для формулы.

    Формулы могут включать следующие операторы:
    - ¬, ! - логическое отрицание
    - &, ^ - логическое И
    - |, v - логическое ИЛИ
    """

    def __init__(self):
        self.window = tk.Tk()
        self.window.title("КНФ решатель")
        self.window.geometry("500x400")
        self.window.configure(bg="#E8F0FE")

        # Поле для ввода формулы
        self.formula_label = tk.Label(self.window, text="Введите формулу логики:", bg="#E8F0FE", font=("Arial", 12))
        self.formula_label.pack(pady=10)

        self.formula_entry = tk.Entry(self.window, width=50, font=("Arial", 12))
        self.formula_entry.pack(pady=10)

        # Кнопка для проверки противоречивости
        self.check_button = tk.Button(self.window, text="Проверить противоречивость", command=self.check_consistency,
                                      bg="#FF6464", fg="white", font=("Arial", 12))
        self.check_button.pack(pady=5)

        # Кнопка для поиска всех истинных интерпретаций
        self.find_button = tk.Button(self.window, text="Найти все истинные интерпретации",
                                     command=self.find_true_interpretations, bg="#4CAF50", fg="white",
                                     font=("Arial", 12))
        self.find_button.pack(pady=5)

        # Кнопка для вывода таблицы истинности
        self.truth_table_button = tk.Button(self.window, text="Вывести таблицу истинности",
                                            command=self.show_truth_table, bg="#FFC107", fg="white", font=("Arial", 12))
        self.truth_table_button.pack(pady=5)

        # Поле для вывода результата
        self.result_text = tk.Text(self.window, height=10, width=50, font=("Arial", 12), bg="#F8F9FA")
        self.result_text.pack(pady=10)

        self.window.mainloop()

    def preprocess_formula(self, formula):
        """
        Преобразование формулы:
        1. Добавляем конъюнкцию, если нет операций между операндами.
        2. Заменяем символы &, !, | на ^, ¬ и v соответственно.

        @param formula Формула в строковом формате.
        @return Обработанная строка формулы.
        """
        formula = formula.replace(" ", "")  # Убираем пробелы
        formula = re.sub(r"([A-Za-z])([A-Za-z])", r"\1&\2", formula)
        formula = formula.replace("&", "^").replace("!", "¬").replace("|", "v")
        return formula

    def evaluate_formula(self, formula, interpretation):
        """
        Оценка истинности формулы на основе интерпретации.

        @param formula Строка с логической формулой.
        @param interpretation Словарь с интерпретацией переменных.
        @return Булево значение результата формулы.
        """
        stack = []
        for token in formula:
            if token in interpretation:
                stack.append(interpretation[token])
            elif token == "¬":
                stack.append(not stack.pop())
            elif token == "^":
                b = stack.pop()
                a = stack.pop()
                stack.append(a and b)
            elif token == "v":
                b = stack.pop()
                a = stack.pop()
                stack.append(a or b)
        return stack[0]

    def parse_formula(self, formula):
        """
        Парсер для преобразования строки формулы в список токенов с поддержкой приоритетов операций.

        @param formula Логическая формула в строковом формате.
        @return Список токенов формулы.
        """
        def precedence(op):
            if op == '¬':
                return 3
            if op == '^':
                return 2
            if op == 'v':
                return 1
            return 0

        def apply_operator(operators, values):
            operator = operators.pop()
            if operator == '¬':
                val = values.pop()
                values.append(not val)
            else:
                right = values.pop()
                left = values.pop()
                if operator == '^':
                    values.append(left and right)
                elif operator == 'v':
                    values.append(left or right)

        tokens = re.findall(r'[A-Za-z]|\^|v|¬|\(|\)', formula)
        values = []
        operators = []
        for token in tokens:
            if token.isalpha():
                values.append(token)
            elif token == '(':
                operators.append(token)
            elif token == ')':
                while operators and operators[-1] != '(':
                    apply_operator(operators, values)
                operators.pop()
            else:
                while (operators and operators[-1] != '(' and
                       precedence(operators[-1]) >= precedence(token)):
                    apply_operator(operators, values)
                operators.append(token)

        while operators:
            apply_operator(operators, values)

        return values

    def show_truth_table(self):
        """Формирует и выводит таблицу истинности для формулы."""
        formula_str = self.formula_entry.get()
        formula = self.preprocess_formula(formula_str)
        parsed_formula = self.parse_formula(formula)

        variables = sorted(self.extract_variables(formula))  # Переменные в отсортированном порядке
        interpretations = list(product([True, False], repeat=len(variables)))

        self.result_text.delete(1.0, tk.END)
        self.result_text.insert(tk.END, "Таблица истинности:\n")
        headers = "  ".join(variables) + " | Результат\n"
        self.result_text.insert(tk.END, headers)

        for values in interpretations:
            interpretation = dict(zip(variables, values))
            result = self.is_formula_true(parsed_formula, interpretation)
            row = "  ".join(["T" if v else "F" for v in values]) + " | " + ("T" if result else "F") + "\n"
            self.result_text.insert(tk.END, row)

    def is_formula_true(self, formula, interpretation):
        """Проверка, истинна ли формула при заданной интерпретации."""
        return self.evaluate_formula(formula, interpretation)

    def extract_variables(self, formula):
        """Извлечение всех переменных из формулы."""
        return {token for token in re.findall(r'[A-Za-z]', formula)}

    def check_consistency(self):
        """Проверка противоречивости формулы."""
        formula_str = self.formula_entry.get()
        formula = self.preprocess_formula(formula_str)
        parsed_formula = self.parse_formula(formula)

        variables = self.extract_variables(formula)
        interpretations = product([True, False], repeat=len(variables))

        for values in interpretations:
            interpretation = dict(zip(variables, values))
            if self.is_formula_true(parsed_formula, interpretation):
                self.result_text.delete(1.0, tk.END)
                self.result_text.insert(tk.END, "Формула выполнима. Нет противоречия.\n")
                return

        self.result_text.delete(1.0, tk.END)
        self.result_text.insert(tk.END, "Формула противоречива (невыполнима).\n")

    def find_true_interpretations(self):
        """Найти все интерпретации, при которых формула истинна."""
        formula_str = self.formula_entry.get()
        formula = self.preprocess_formula(formula_str)
        parsed_formula = self.parse_formula(formula)

        variables = sorted(self.extract_variables(formula))
        interpretations = product([True, False], repeat=len(variables))

        true_interpretations = []
        for values in interpretations:
            interpretation = dict(zip(variables, values))
            if self.is_formula_true(parsed_formula, interpretation):
                true_interpretations.append(interpretation)

        if true_interpretations:
            self.result_text.delete(1.0, tk.END)
            self.result_text.insert(tk.END, "Истинные интерпретации:\n")
            for interp in true_interpretations:
                self.result_text.insert(tk.END, str(interp) + "\n")
        else:
            self.result_text.delete(1.0, tk.END)
            self.result_text.insert(tk.END, "Нет истинных интерпретаций.\n")


if __name__ == "__main__":
    app = LogicSolver()
