# C言語演習ガイド
## ビルド・単体テスト・静的コード解析・CI を一通り体験する

---

> **この演習の目的**
> 「コードを書く」だけでなく、**品質を担保する仕組みを自分で構築する**体験をする。
> 各ステップで何をやっているのかを理解しながら進めること。

---

## 演習の全体像

```
ステップ1: ビルド          → Makefile を理解してビルドを通す
ステップ2: 単体テスト       → Unity でテストを書いて実行する
ステップ3: 静的コード解析   → cppcheck でコードの問題を検出する
ステップ4: カバレッジ計測   → テストの網羅率を可視化する
ステップ5: CI構築          → GitHub Actions で自動化する
```

---

## ステップ1：ビルド

### 目的

Makefile を読み解き、コンパイル・リンクの流れを理解する。

### やること

#### 課題1-1：まずビルドして動かす

```bash
cd c-training
make build/calculator
./build/calculator
```

以下の出力が出れば成功です。

```
=== Calculator Demo ===
10 + 3 = 13
10 - 3 = 7
10 * 3 = 30
10 / 3 = 3
10 / 0 = エラー（ゼロ除算）
```

#### 課題1-2：Makefile を読む

`Makefile` を開いて以下を確認してください。

| 変数 | 意味 | C言語実務での相当物 |
|------|------|-------------------|
| `CC = gcc` | 使用するコンパイラ | ツールチェーンの指定 |
| `CFLAGS = -Wall -Wextra -std=c11` | コンパイルオプション | ビルドオプション |
| `SRCS = src/calculator.c` | コンパイル対象のソース | ソースファイル一覧 |

**確認ポイント**：

```bash
# -Wall -Wextra を外すと警告が出なくなることを確認
# Makefile の CFLAGS を一時的に変えてみる
CFLAGS = -std=c11   # 警告オプションを外した状態
make clean && make build/calculator
```

#### 課題1-3：意図的にコンパイルエラーを起こす

`src/calculator.c` の `calc_add` を以下のように書き換えてみてください。

```c
int calc_add(int a, int b)
{
    return a + b   /* セミコロンを消す */
}
```

```bash
make build/calculator
```

エラーメッセージを読んで「どのファイルの何行目でどんなエラーが出ているか」を確認してください。確認できたら元に戻してください。

#### 課題1-4：警告を出す

`src/main.c` に以下を追加して、未使用変数の警告を体験してください。

```c
int main(void)
{
    int unused_var = 42;   /* 使わない変数を追加 */
    ...
}
```

```bash
make clean && make build/calculator
```

`-Wunused-variable` の警告が出ることを確認してください。確認できたら元に戻してください。

---

## ステップ2：単体テスト

### 目的

Unity を使ってテストを書き、実行する流れを体験する。
「テストが通る」だけでなく「テストが失敗する」状態も意図的に作り、
テストの意味を理解する。

### やること

#### 課題2-1：テストを実行する

```bash
make test
```

以下の出力が出ることを確認してください。

```
----- test_calculator.c -----

14 Tests  0 Failures  0 Ignored
OK
```

#### 課題2-2：テストコードを読む

`tests/test_calculator.c` を開いて以下を確認してください。

```c
/* テスト関数の構造 */
void test_add_positive_numbers(void)
{
    TEST_ASSERT_EQUAL_INT(13, calc_add(10, 3));
    /*                    ↑期待値  ↑実際の値  */
}
```

**確認ポイント**：
- `UnityBegin` / `UnityEnd` の役割は何か
- `RUN_TEST(関数名)` で何が起きるか
- `setUp` / `tearDown` はいつ呼ばれるか（今回は空だが概念を理解する）

#### 課題2-3：テストを意図的に失敗させる

`src/calculator.c` の `calc_add` を壊してみてください。

```c
int calc_add(int a, int b)
{
    return a - b;   /* わざと間違える */
}
```

```bash
make test
```

以下のような出力が出ることを確認してください。

```
FAIL: tests/test_calculator.c:XX: test_add_positive_numbers - Expected 13 but was 7
```

「どのテスト関数の何行目で何が期待値と違ったか」が分かることを確認してください。
確認できたら元に戻して `make test` が全件通ることを確認してください。

#### 課題2-4：テストを自分で追加する

以下のテストケースを `tests/test_calculator.c` に自分で追加してください。

```
追加するテスト：

(1) test_add_large_numbers
    1000 + 2000 = 3000 になることを確認する

(2) test_subtract_negative_from_negative
    -5 - (-3) = -2 になることを確認する

(3) test_multiply_large_numbers
    100 * 100 = 10000 になることを確認する

(4) test_divide_negative_dividend
    -10 / 3 = -3 になることを確認する（整数除算・切り捨て）
```

追加したら `RUN_TEST(...)` に登録して実行してください。

```bash
make test
# 18 Tests  0 Failures  0 Ignored  になれば成功
```

#### 課題2-5：境界値テストを追加する（発展）

以下の境界値についてテストを追加してください。

```
(1) INT_MAX（2147483647）同士の加算
    → オーバーフローが起きる（現在の実装は検出しない）
    → まずテストを書いて「現状の挙動」を記録することが目的

(2) calc_divide で b = -1 のとき
    → -10 / -1 = 10 になることを確認する

(3) calc_divide で a = 0, b = 0 のとき
    → CALC_ERR_DIV_ZERO が返ることを確認する
```

**ヒント**：`INT_MAX` は `<limits.h>` をインクルードすると使えます。

```c
#include <limits.h>
void test_add_overflow(void)
{
    /* INT_MAX + 1 の挙動を記録する（未定義動作の観察） */
    int result = calc_add(INT_MAX, 1);
    /* 期待値はあえて決めず、現状の挙動を printf で観察する */
    printf("INT_MAX + 1 = %d\n", result);
    TEST_PASS();   /* このテストは通過扱い（観察目的） */
}
```

---

## ステップ3：静的コード解析

### 目的

コードを実行せずに問題を検出するツール（cppcheck）の使い方を覚える。
「コンパイルが通る = 正しい」ではないことを理解する。

### やること

#### 課題3-1：まず解析を実行する

```bash
make lint
```

現状のコードは問題なく通ることを確認してください。

```
=== 静的コード解析 (cppcheck) ===
解析完了（エラーなし）
```

#### 課題3-2：問題のあるコードを書いて検出させる

`src/calculator.c` に以下の問題コードを追加してみてください。

**問題1：NULLポインタの未チェック**

```c
/* 以下を calc_divide の下に追加 */
int calc_bad_divide(int a, int b, int *result)
{
    *result = a / b;   /* result が NULL かどうか確認していない */
    return CALC_OK;
}
```

`src/calculator.h` にも宣言を追加してください。

```c
int calc_bad_divide(int a, int b, int *result);
```

```bash
make lint
```

cppcheck が `Null pointer dereference` や `Division by zero` を検出することを確認してください。確認できたら追加したコードを削除して `make lint` が通ることを確認してください。

#### 課題3-3：解析オプションを理解する

`Makefile` の `lint` ターゲットを確認してください。

```makefile
cppcheck --enable=all \
         --suppress=missingIncludeSystem \
         --error-exitcode=1 \
         -I$(SRC_DIR) \
         $(SRC_DIR)/calculator.c $(SRC_DIR)/main.c
```

| オプション | 意味 |
|-----------|------|
| `--enable=all` | 全チェック項目を有効化 |
| `--suppress=missingIncludeSystem` | システムヘッダの警告を抑制 |
| `--error-exitcode=1` | エラー検出時に終了コード1を返す（CIでビルド失敗にするため） |
| `-I$(SRC_DIR)` | インクルードパスを指定 |

**確認ポイント**：`--error-exitcode=1` を外すと、エラーが出ても `make lint` が成功扱いになることを確認してください。

---

## ステップ4：カバレッジ計測

### 目的

テストが「どの行を実行したか」を可視化し、テストの抜け漏れを発見する。
Pythonの pytest-cov と同じ概念を C言語でも体験する。

### やること

#### 課題4-1：カバレッジを計測する

```bash
make coverage
open build/cov/html/index.html
```

ブラウザでレポートが開きます。`calculator.c` の行ごとの実行状況を確認してください。

- **緑の行**：テストで実行された行
- **赤の行**：テストで実行されなかった行（未到達）

#### 課題4-2：未到達行を探す

現状のテストでカバーされていない行を探してください。

**ヒント**：`calc_divide` 関数の中に未到達になりやすい分岐があります。
どの条件を満たすテストが不足しているか考えてください。

#### 課題4-3：カバレッジを上げる

未到達行をカバーするテストを追加して、再度計測してください。

```bash
make coverage
open build/cov/html/index.html
```

`calculator.c` のカバレッジが100%になることを目指してください。

**確認ポイント**：どんなテストを追加したら未到達行に到達できましたか？

#### 課題4-4：カバレッジ100%の意味を考える

カバレッジが100%になったとして、以下を考えてください。

```
Q: カバレッジ100%はバグがないことを保証するか？

例：以下の実装にバグがある場合、カバレッジ100%でも検出できるか？

int calc_add(int a, int b)
{
    return a + b + 1;   /* +1 が余分についている */
}

→ テストが TEST_ASSERT_EQUAL_INT(13, calc_add(10, 3)) であれば
  14 が返るため FAIL になる。→ 検出できる。

→ テストが TEST_ASSERT_TRUE(calc_add(10, 3) > 0) であれば
  14 > 0 は真なので PASS になる。→ 検出できない。

結論：カバレッジ100%は「全行が実行された」ことを示すが、
     「テストが正しい期待値を持っているか」は別の問題。
```

---

## ステップ5：CI構築

### 目的

ローカルで手動実行していたビルド・テスト・静的解析を、
`git push` のたびに自動で実行される仕組みを作る。

### やること

#### 課題5-1：リポジトリを GitHub に作成して push する

```bash
cd c-training
git init
git add .
git commit -m "feat: 初回コミット - calculator ライブラリ"

# GitHub で新規リポジトリ「c-training」を作成してから
git remote add origin https://github.com/YusukeHarada/c-training.git
git branch -M main
git push -u origin main
```

#### 課題5-2：GitHub Actions のログを確認する

GitHub のリポジトリページで `Actions` タブを開いてください。

- ワークフローが自動で起動していることを確認する
- 各ステップ（ビルド・テスト・静的解析・カバレッジ）のログを開いて確認する
- 全ステップが緑（成功）になることを確認する

#### 課題5-3：CIを意図的に失敗させる

`src/calculator.c` の `calc_add` を壊してコミットしてください。

```c
int calc_add(int a, int b)
{
    return a - b;   /* わざと間違える */
}
```

```bash
git add src/calculator.c
git commit -m "test: CIが失敗することを確認するためのバグ導入"
git push
```

GitHub Actions が赤くなり、「単体テスト」のステップで失敗することを確認してください。

確認できたら元に戻してください。

```bash
# calculator.c を元に戻す
git revert HEAD
git push
# または
# 手動で修正して
git add src/calculator.c
git commit -m "fix: calc_add のバグを修正"
git push
```

#### 課題5-4：CIのワークフローを読む

`.github/workflows/ci.yml` を開いて以下を確認してください。

```yaml
on:
  push:
    branches: [ main ]    # main ブランチへの push で起動
  pull_request:
    branches: [ main ]    # main への PR 作成でも起動

jobs:
  build-and-test:
    runs-on: ubuntu-latest   # Ubuntu 環境で実行

    steps:
      - name: 依存ツールのインストール
        run: |
          sudo apt-get install -y gcc make cppcheck lcov
          # macOS ではなく Ubuntu なので apt-get を使う
```

**確認ポイント**：

- `runs-on: ubuntu-latest` は何を意味するか
- `steps` の順番を入れ替えるとどうなるか（例：テストの前に静的解析を実行する）
- カバレッジレポートが `Artifacts` としてダウンロードできることを確認する

---

## 発展課題：自分でライブラリを拡張する

ここまでのステップが完了したら、以下を自分で実装してみてください。

### 発展1：calc_modulo（剰余）を追加する

```c
/* 追加する関数 */
int calc_modulo(int a, int b, int *result);
/* b が 0 のとき CALC_ERR_DIV_ZERO を返す */
```

**手順**：
1. `calculator.h` に宣言を追加
2. `calculator.c` に実装を追加
3. `test_calculator.c` にテストを追加（正常系・ゼロ除算・負の数）
4. `make test` で全件通ることを確認
5. `make lint` で静的解析を通す
6. `make coverage` でカバレッジ100%を確認
7. `git push` でCIが通ることを確認

### 発展2：エラー処理を calc_add / calc_subtract にも追加する

現状の `calc_add` はオーバーフローを検出しません。
以下の仕様で実装してみてください。

```c
/* エラーコードを追加 */
#define CALC_ERR_OVERFLOW -2

/* 新しいシグネチャ */
int calc_add_safe(int a, int b, int *result);
/* a + b が INT_MAX を超える、または INT_MIN を下回る場合は
   CALC_ERR_OVERFLOW を返す */
```

**ヒント**：オーバーフロー検出は加算前に判定します。

```c
/* 加算後にチェックするのは UB（未定義動作）になるため NG */
/* 加算前に以下のように判定する */
if (b > 0 && a > INT_MAX - b) { /* オーバーフロー */ }
if (b < 0 && a < INT_MIN - b) { /* アンダーフロー */ }
```

---

## 振り返りチェックリスト

各ステップが完了したら確認してください。

```
ステップ1 ビルド
□ make build/calculator が通った
□ Makefile の各変数（CC, CFLAGS, SRCS）の意味を理解した
□ コンパイルエラーのメッセージを読めた
□ -Wall の警告を体験した

ステップ2 単体テスト
□ make test で 14 Tests OK が出た
□ テストを意図的に失敗させてエラーメッセージを読めた
□ テストを自分で追加して登録できた
□ 境界値テストの意味を理解した

ステップ3 静的コード解析
□ make lint が通った
□ 問題のあるコードを書いて cppcheck に検出させた
□ --error-exitcode=1 の意味を理解した

ステップ4 カバレッジ
□ HTML レポートを開いて未到達行を確認した
□ 未到達行をカバーするテストを追加した
□ calculator.c のカバレッジが 100% になった
□ カバレッジ100%の限界を理解した

ステップ5 CI
□ GitHub にリポジトリを作って push した
□ Actions でワークフローが動くことを確認した
□ 意図的にテストを失敗させて CI が赤くなることを確認した
□ 修正 push で CI が緑に戻ることを確認した
□ カバレッジレポートを Artifacts からダウンロードした
```

---

*c-training 演習ガイド — calculator ライブラリを題材にしたビルド・テスト・CI 体験*
