# MyDataBasae
база данных на С++
В базе данных используется хэш-таблица и аналог vector. Файлы находятся в папке include.
## Описание команд:
### вывод данных:
SELECT таблица1.колонка1, таблица2.колонка1 
FROM таблица1, таблица2 
WHERE таблица1.колонка1 = таблица2.колонка2AND таблица1.колонка2 = 'string'
### удаление:
DELETE FROM таблица1 
WHERE таблица1.колонка1 = '123'
### вставка:
INSERT INTO таблица1 VALUES ('somedata', '12345')
