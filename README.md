# Технопарк
Задания по дисциплине "Углубленное программирование на с++

## Задание 1
Чтобы показать информацию о пришедшем письме, нужно сначала её найти. 
Для этого в файле письма необходимо найти специальные заголовки. 

Составить программу построчной фильтрации текста.
Суть фильтра — отбор строк, начинающихся с одного из следующих выражений: «Date:», «From:»,«To:», «Subject:».
Текстовые строки подаются на стандартный ввод программы, результат программы должен подаваться на стандартный вывод.

Процедура фильтрации должна быть оформлена в виде отдельной функции, которой подается на вход массив строк, выделенных в динамической памяти и его длина.
На выходе функция возвращает указатель на NULL-терминированный массив с найденными строками (последним элементом массива добавлен NULL для обозначения, что данных больше нет).

Программа должна уметь обрабатывать возникающие ошибки (например, ошибки выделения памяти). В случае возникновения ошибки нужно вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы. 

ВАЖНО! Программа в любом случае должна возвращать 0. Не пишите return -1, exit(1) и т.п. Даже если обнаружилась какая-то ошибка, все равно необходимо вернуть 0! (и напечатать [error] в stdout).

Пример:

| Input | Output |
| --- | --- |
| From | admin |
| Subject | hi |
| Sender | admin |
| From | admin |
| Subject | hi |

