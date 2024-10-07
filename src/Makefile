CC= gcc
FLAGS= -Wall -Werror -Wextra
FLAGS_GCOV= -fprofile-arcs -ftest-coverage

ifeq ($(shell uname -s), Darwin)
	FLAGS_TESTS= -lcheck
endif
ifeq ($(shell uname -s), Linux)
	FLAGS_TESTS= -lcheck -lm -lsubunit
endif

DIR_REPORT= ./report
DIR_INFO= ./info

NAME_C_FORLIB= s21_decimal.c
NAME_O_FORLIB= s21_decimal.o
NAME_LIB= s21_decimal.a
NAME_TEST= test_s21_decimal
NAME_GCOVTEST= gcov_s21_decimal

SRCS = 	s21_is_less.c\
		s21_is_less_or_equal.c\
		s21_is_greater.c\
		s21_is_greater_or_equal.c\
		s21_is_equal.c\
		s21_is_not_equal.c\
		s21_add.c\
		s21_sub.c\
		s21_mul.c\
		s21_div.c\
		s21_from_decimal_to_float.c\
		s21_from_decimal_to_int.c\
		s21_from_float_to_decimal.c\
		s21_from_int_to_decimal.c\
		s21_truncate.c\
		s21_negate.c\
		s21_floor.c\
		s21_round.c\
		binary_operations.c\

OBJS		= $(SRCS:.c=.o)

TESTDIR = tests
TEST = 	$(TESTDIR)/test_decimal.c\
		$(TESTDIR)/add.c\
		$(TESTDIR)/sub.c\
		$(TESTDIR)/test_mul.c\
		$(TESTDIR)/test_div.c\
		$(TESTDIR)/less.c\
		$(TESTDIR)/less_or_equal.c\
		$(TESTDIR)/greater.c\
		$(TESTDIR)/greater_or_equal.c\
		$(TESTDIR)/equal.c\
		$(TESTDIR)/not_equal.c\
		$(TESTDIR)/test_from_decimal_to_float.c\
		$(TESTDIR)/test_from_decimal_to_int.c\
		$(TESTDIR)/test_from_float_to_decimal.c\
		$(TESTDIR)/test_from_int_to_decimal.c\
		$(TESTDIR)/floor.c\
		$(TESTDIR)/test_truncate.c\
		$(TESTDIR)/test_negate.c\
		$(TESTDIR)/test_round.c\

all: $(NAME_LIB) test gcov_report

$(NAME_LIB): 
	@$(CC) -c $(SRCS) $(FLAGS)
	@ar rcs $(NAME_LIB) $(OBJS)
	@rm -rf *.o

test:
	@$(CC) -o $(NAME_TEST) $(SRCS) $(TEST) $(FLAGS_TESTS) $(FLAGS)


gcov_report:
	@$(CC) -o $(NAME_GCOVTEST) $(SRCS) $(TEST) $(FLAGS_TESTS) $(FLAGS) $(FLAGS_GCOV)
	@chmod +x *
	@./$(NAME_GCOVTEST)
	@mkdir $(DIR_INFO) $(DIR_REPORT)
	@gcov ./* ./
	@mv *.gcno *.gcda *.gcov $(DIR_INFO)/
	@lcov -d $(DIR_INFO) -c -o $(DIR_INFO)/lcov.info
	@genhtml -o $(DIR_REPORT) $(DIR_INFO)/lcov.info
	@open $(DIR_REPORT)/index.html

check: test
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config *.c *.h */*.c */*.h
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -style=Google -n *.c *.h */*.c */*.h
	clang-format -style=Google -i *.c *.h */*.c */*.h
	@rm -rf .clang-format

clean:
	@rm -rf *.o *.a *.gcno *.gcda *.gcov *.gch a.out $(NAME_TEST) $(NAME_GCOVTEST) $(DIR_INFO) $(DIR_REPORT) 

git_push:
	git add .
	git commit -m'.'
	git push


rebuid: clean test
