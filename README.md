# Logger Analyzer

## Description

`Logger Analyzer` is a C project designed to model an AI agent that analyzes logs from one or more files and makes decisions based on pre-defined commands. This project demonstrates the use of multi-threading to handle log analysis concurrently and provides a framework for future enhancements.

## Features

- **Multi-threading:** Utilizes threads to analyze log files concurrently.
- **Signal Handling:** Gracefully handles termination signals to cancel threads and clean up resources.
- **Dynamic File Handling:** Reads directories and files dynamically based on user input.
- **Debugging Output:** Displays detailed information about the agent's data and actions.

## Usage

Compile the project using `gcc`:

```sh
gcc -o logger_analyzer main.c -lpthread
```

Run the executable with the following command line arguments:

```sh
./logger_analyzer <target_string> <action_command>
```

- `<target_string>`: The string to search for in log files.
- `<action_command>`: The command to execute when the target string is found.

## Example

```sh
./logger_analyzer "ERROR" "process_error.sh"
```

## How It Works

1. **Signal Handling:** The program sets up a signal handler to clean up resources and terminate threads upon receiving a signal (e.g., `CTRL+C`).
2. **Directory Input:** Prompts the user to enter paths to directories containing log files.
3. **Thread Creation:** For each directory, it creates a thread to analyze the newest file and execute the specified command if the target string is found.
4. **Thread Synchronization:** Waits for all threads to complete before exiting.
5. **Resource Cleanup:** Frees allocated memory and resources.

## Future Improvements

- **Decision Trees:** Implement decision tree logic to make more complex decisions based on log data. This could involve creating a decision-making framework where different types of log entries lead to different actions.
- **Configuration File Parsing:** Add support for configuration files to allow users to define log analysis parameters and actions in a more flexible manner.
- **Enhanced Logging and Monitoring:** Improve logging to include more detailed and structured information about the agent's activities and decisions.
- **Error Handling and Robustness:** Implement better error handling to manage various edge cases and potential failures more gracefully.
- **User Interface:** Develop a user interface (CLI or GUI) to make the tool more user-friendly and easier to configure.

## Contributing

Feel free to contribute to the project by submitting issues or pull requests. Your feedback and improvements are welcome!