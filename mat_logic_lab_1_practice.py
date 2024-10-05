import tkinter as tk
from itertools import product
import re


class LogicSolver:
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

        # Поле для вывода результата
        self.result_text = tk.Text(self.window, height=10, width=50, font=("Arial", 12), bg="#F8F9FA")
        self.result_text.pack(pady=10)

        self.window.mainloop()

    def preprocess_formula(self, formula):
        """
        Преобразование формулы:
        1. Добавляем конъюнкцию, если нет операций между операндами.
        2. Заменяем символы &, ! на ^ и ¬, соответственно.
        """
        formula = formula.replace(" ", "")  # Убираем пробелы

        # Обрабатываем неявные конъюнкции (например, AB -> A & B)
        formula = re.sub(r"([A-Za-z])([A-Za-z])", r"\1&\2", formula)

        # Заменяем логические операторы на внутренние символы
        formula = formula.replace("&", "^").replace("!", "¬")

        return formula

    def evaluate_formula(self, formula, interpretation):
        """Проверка истинности формулы с учетом интерпретации"""
        stack = []
        for token in formula:
            if token in interpretation:  # Если это переменная
                stack.append(interpretation[token])
            elif token == "¬":  # Логическое НЕ
                stack.append(not stack.pop())
            elif token == "^":  # Логическое И
                b = stack.pop()
                a = stack.pop()
                stack.append(a and b)
            elif token == "v":  # Логическое ИЛИ
                b = stack.pop()
                a = stack.pop()
                stack.append(a or b)
        return stack[0]

    def parse_formula(self, formula):
        """
        Парсер для преобразования строки формулы в список токенов с поддержкой приоритетов операций.
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

        # Алгоритм "сортировочной станции" для разборки формулы
        tokens = re.findall(r'[A-Za-z]|\^|v|¬|\(|\)', formula)
        values = []
        operators = []
        for token in tokens:
            if token.isalpha():  # Переменная
                values.append(token)
            elif token == '(':
                operators.append(token)
            elif token == ')':
                while operators and operators[-1] != '(':
                    apply_operator(operators, values)
                operators.pop()  # Убираем '('
            else:  # Это оператор
                while (operators and operators[-1] != '(' and
                       precedence(operators[-1]) >= precedence(token)):
                    apply_operator(operators, values)
                operators.append(token)

        while operators:
            apply_operator(operators, values)

        return values

    def is_formula_true(self, formula, interpretation):
        """Проверка, истинна ли формула при заданной интерпретации"""
        return self.evaluate_formula(formula, interpretation)

    def extract_variables(self, formula):
        """Извлечение всех переменных из формулы"""
        return {token for token in re.findall(r'[A-Za-z]', formula)}

    def check_consistency(self):
        """Проверка противоречивости формулы"""
        formula_str = self.formula_entry.get()
        formula = self.preprocess_formula(formula_str)
        parsed_formula = self.parse_formula(formula)

        variables = self.extract_variables(formula)  # Получаем переменные
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
        """Найти все интерпретации, при которых формула истинна"""
        formula_str = self.formula_entry.get()
        formula = self.preprocess_formula(formula_str)
        parsed_formula = self.parse_formula(formula)

        variables = self.extract_variables(formula)  # Получаем переменные
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
