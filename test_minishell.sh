#!/bin/bash

# Path to your minishell executable
MINISHELL=./minishell

# Function to run a command in minishell and check the output
run_test() {
  local command="$1"
  local expected="$2"

  # Run the command in minishell and capture the output
  output=$(echo "$command" | $MINISHELL | tr -d '\r')

  # Check if the output matches the expected output
  if [ "$output" == "$expected" ]; then
    echo "Test passed: $command"
  else
    echo "Test failed: $command"
    echo "Expected: $expected"
    echo "Got: $output"
  fi
}

# Test echo command
run_test "echo Hello, World!" "Hello, World!"

# Test pwd command (assuming your initial directory is your home directory)
expected_pwd=$(pwd)
run_test "pwd" "$expected_pwd"

# Test cd command
run_test "cd /tmp && pwd" "/tmp"

# Test ls command
run_test "ls / | grep bin" "bin"

# Test environment variable setting and retrieval
run_test "export TEST_VAR=42 && echo \$TEST_VAR" "42"

# Test input redirection
echo "Hello from file" > input.txt
run_test "cat < input.txt" "Hello from file"
rm input.txt

# Test output redirection
run_test "echo Hello > output.txt" ""
if [ "$(cat output.txt)" == "Hello" ]; then
  echo "Test passed: output redirection"
else
  echo "Test failed: output redirection"
fi
rm output.txt

# Test pipe
run_test "echo Hello | grep H" "Hello"

# Test exit command
run_test "exit" ""

echo "All tests completed."
