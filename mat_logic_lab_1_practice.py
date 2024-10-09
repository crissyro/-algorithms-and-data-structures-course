"""@made by ujent"""

import re
import itertools
import tkinter as tk
from tkinter import scrolledtext

class TruthTableApp:
    def __init__(self, root):
        """
        Инициализация интерфейса приложения и основных элементов.
        """
        self.root = root
        self.root.title("Решатель КНФ")

        # Поле ввода для формулы
        label_formula = tk.Label(root, text="Введите формулу:")
        label_formula.pack(pady=5)

        self.entry_formula = tk.Entry(root, width=50)
        self.entry_formula.pack(pady=5)

        # Режим отображения (все строки, только истинные, только ложные)
        self.mode_var = tk.IntVar(value=2)

        frame_mode = tk.Frame(root)
        frame_mode.pack(pady=5)

        label_mode = tk.Label(frame_mode, text="Выберите задачу:")
        label_mode.pack(side=tk.LEFT)

        radio_all = tk.Radiobutton(frame_mode, text="Таблица истинности", variable=self.mode_var, value=2)
        radio_all.pack(side=tk.LEFT)

        radio_true = tk.Radiobutton(frame_mode, text="Только истинные интерпретации", variable=self.mode_var, value=1)
        radio_true.pack(side=tk.LEFT)

        radio_false = tk.Radiobutton(frame_mode, text="Проверка на противоречивость", variable=self.mode_var, value=0)
        radio_false.pack(side=tk.LEFT)

        # Кнопка для расчета таблицы истинности
        button_calculate = tk.Button(root, text="Вывести результат", command=self.show_truth_table)
        button_calculate.pack(pady=10)

        # Поле вывода таблицы истинности (с возможностью прокрутки)
        self.output_text = scrolledtext.ScrolledText(root, width=60, height=20)
        self.output_text.pack(pady=10)

    def formating_string(self, string):
        """
        Форматирует входную строку, добавляя операторы и заменяя символы.

        @param string: Входная строка с логической формулой.
        @return: Отформатированная строка.
        """
        string = re.sub(r"([A-Za-z])([A-Za-z])", r"\1&\2", string)
        string = string.upper().replace(" ", "")
        return string.replace("!", "¬").replace("|", "v")

    def precedence(self, op):
        """
        Возвращает приоритет оператора.

        @param op: Символ оператора.
        @return: Приоритет оператора.
        """
        if op == '|':
            return 1
        if op == '&':
            return 2
        if op == '!':
            return 3
        return 0

    def infix_to_postfix(self, expression):
        """
        Преобразует инфиксное выражение в постфиксное.

        @param expression: Инфикcное выражение.
        @return: Постфиксное выражение в виде списка токенов.
        """
        stack = []
        output = []

        for char in expression:
            if char.isalnum():
                output.append(char)
            elif char == '(':
                stack.append(char)
            elif char == ')':
                while stack and stack[-1] != '(':
                    output.append(stack.pop())
                stack.pop()
            else:
                while stack and self.precedence(stack[-1]) >= self.precedence(char):
                    output.append(stack.pop())
                stack.append(char)
        while stack:
            output.append(stack.pop())

        return output

    def calculate_postfix(self, expression, variable_values):
        """
        Вычисляет значение постфиксного выражения.

        @param expression: Постфиксное выражение.
        @param variable_values: Словарь значений переменных.
        @return: Значение выражения (True или False).
        """
        stack = []

        for token in expression:
            if token == '!':
                operand = stack.pop()
                stack.append(not operand)
            elif token == '&':
                operand2 = stack.pop()
                operand1 = stack.pop()
                stack.append(operand1 and operand2)
            elif token == '|':
                operand2 = stack.pop()
                operand1 = stack.pop()
                stack.append(operand1 or operand2)
            else:
                if token in variable_values:
                    stack.append(variable_values[token])
                else:
                    stack.append(bool(int(token)))

        return stack.pop()

    def truth_table(self, expression, fstring, mode=2):
        """
        Генерирует таблицу истинности для логической формулы.

        @param expression: Постфиксное выражение.
        @param fstring: Исходная формула для отображения.
        @param mode: Режим отображения (0 - противоречивость, 1 - только истинные, 2 - все строки).
        @return: Строка с таблицей истинности или сообщение о противоречивости.
        """
        variables = sorted({token for token in expression if token.isalpha()})
        combinations = list(itertools.product([False, True], repeat=len(variables)))

        result_table = []
        contradictory = True  # Флаг для проверки противоречивости

        header = ' | '.join(variables) + ' | ' + fstring
        result_table.append(header)
        result_table.append('-' * (len(variables) * 4 + 10))

        for combination in combinations:
            variable_values = dict(zip(variables, combination))
            result = self.calculate_postfix(expression, variable_values)

            if mode == 0:
                if result:
                    contradictory = False
            elif mode == result or mode == 2:
                row = ' | '.join(str(int(variable_values[var])) for var in variables)
                result_table.append(f'{row} | {int(result)}')

        if mode == 0:
            return "Формула является противоречивой." if contradictory else "Формула не является противоречивой."

        return '\n'.join(result_table)

    def show_truth_table(self):
        """
        Обрабатывает ввод пользователя и отображает таблицу истинности.
        """
        formula = self.entry_formula.get()
        mode = self.mode_var.get()

        if not formula:
            self.output_text.insert(tk.END, "Введите формулу!\n")
            return

        try:
            postfix_expr = self.infix_to_postfix(formula)
            table = self.truth_table(postfix_expr, formula, mode)
            self.output_text.delete(1.0, tk.END)
            self.output_text.insert(tk.END, table)
        except Exception as e:
            self.output_text.insert(tk.END, f"Ошибка: {e}\n")


# Запуск приложения
if __name__ == "__main__":
    root = tk.Tk()
    app = TruthTableApp(root)
    root.mainloop()
