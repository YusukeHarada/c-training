# Makefile - c-training / calculator
#
# ターゲット一覧:
#   make          : 実行ファイルをビルド
#   make test     : 単体テストをビルド＆実行
#   make coverage : カバレッジ計測（lcov）
#   make lint     : 静的コード解析（cppcheck）
#   make clean    : 生成ファイルを削除
#   make all      : test + coverage + lint を一括実行

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
COVFLAGS = --coverage -fprofile-arcs -ftest-coverage

SRC_DIR   = src
TEST_DIR  = tests
UNITY_DIR = unity
BUILD_DIR = build

SRCS  = $(SRC_DIR)/calculator.c
MAIN  = $(SRC_DIR)/main.c
TESTS = $(TEST_DIR)/test_calculator.c
UNITY = $(UNITY_DIR)/unity.c

# -------------------------------------------------------
# デフォルト：実行ファイルのビルド
# -------------------------------------------------------
$(BUILD_DIR)/calculator: $(SRCS) $(MAIN)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $^ -o $@
	@echo "ビルド完了: $@"

# -------------------------------------------------------
# 単体テスト
# -------------------------------------------------------
.PHONY: test
test: $(BUILD_DIR)/test_calculator
	@echo "\n=== テスト実行 ==="
	./$(BUILD_DIR)/test_calculator

$(BUILD_DIR)/test_calculator: $(SRCS) $(TESTS) $(UNITY)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -DUNITY_INCLUDE_CONFIG_H -I$(SRC_DIR) -I$(UNITY_DIR) $^ -o $@

# -------------------------------------------------------
# カバレッジ計測
# -------------------------------------------------------
.PHONY: coverage
coverage:
	@mkdir -p $(BUILD_DIR)/cov
	$(CC) $(CFLAGS) -DUNITY_INCLUDE_CONFIG_H $(COVFLAGS) -I$(SRC_DIR) -I$(UNITY_DIR) \
	    $(SRCS) $(TESTS) $(UNITY) -o $(BUILD_DIR)/test_cov
	./$(BUILD_DIR)/test_cov
	lcov --capture --directory . \
	     --output-file $(BUILD_DIR)/cov/coverage.info \
	     --ignore-errors inconsistent 2>/dev/null || \
	lcov --capture --directory . \
	     --output-file $(BUILD_DIR)/cov/coverage.info
	lcov --remove $(BUILD_DIR)/cov/coverage.info \
	     '*/unity/*' '*/tests/*' \
	     --output-file $(BUILD_DIR)/cov/coverage_filtered.info
	genhtml $(BUILD_DIR)/cov/coverage_filtered.info \
	        --output-directory $(BUILD_DIR)/cov/html
	@echo "\nカバレッジレポート: $(BUILD_DIR)/cov/html/index.html"

# -------------------------------------------------------
# 静的コード解析
# -------------------------------------------------------
.PHONY: lint
lint:
	@echo "\n=== 静的コード解析 (cppcheck) ==="
	cppcheck --enable=all \
	         --suppress=missingIncludeSystem \
	         --error-exitcode=1 \
	         -I$(SRC_DIR) \
	         $(SRC_DIR)/calculator.c $(SRC_DIR)/main.c
	@echo "解析完了（エラーなし）"

# -------------------------------------------------------
# 一括実行
# -------------------------------------------------------
.PHONY: all
all: test lint
	@echo "\n=== 全チェック完了 ==="

# -------------------------------------------------------
# クリーンアップ
# -------------------------------------------------------
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
	@echo "クリーン完了"
