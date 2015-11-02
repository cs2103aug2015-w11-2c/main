#include "CommandParser.h"

namespace DoLah {
    std::vector<std::string> CommandParser::ADD = { "add" };
    std::vector<std::string> CommandParser::SEARCH = { "search" };
    std::vector<std::string> CommandParser::DONE = { "done" };
    std::vector<std::string> CommandParser::UNDONE = { "undone" };
    std::vector<std::string> CommandParser::EDIT = { "edit" };
    std::vector<std::string> CommandParser::DELETE = { "delete", "del" };
    std::vector<std::string> CommandParser::CLEAR = { "clear" };
    std::vector<std::string> CommandParser::UNDO = { "undo" };

    std::string CommandParser::UNHANDLED_COMMAND_MESSAGE = "Command not handled";
    std::string CommandParser::UNKNOWN_COMMAND_MESSAGE = "Command not recognized";
    std::string CommandParser::TOO_MANY_ARGUMENTS_MESSAGE = "Too many arguments";
    std::string CommandParser::TOO_LITTLE_ARGUMENTS_MESSAGE = "Too little arguments";
    std::string CommandParser::INVALID_TASK_ID_ARGUMENT = "Invalid task ID given";


    CommandParser::CommandParser() {
    }


    CommandParser::~CommandParser() {
    }

    AddTaskCommand CommandParser::parseAdd(std::vector<std::string> inputArr) {
        if (inputArr.size() == 0) {
            throw std::invalid_argument(TOO_LITTLE_ARGUMENTS_MESSAGE);
        }

        AbstractTask* task = TaskParser::parseTask(inputArr);
        return AddTaskCommand(task);
    }

    SetDoneTaskCommand CommandParser::parseSetDone(std::vector<std::string> inputArr) {
        if (inputArr.size() == 0) {
            throw std::invalid_argument(TOO_LITTLE_ARGUMENTS_MESSAGE);
        } else if (inputArr.size() > 1) {
            throw std::invalid_argument(TOO_MANY_ARGUMENTS_MESSAGE);
        }

        int taskID;
        try {
            taskID = std::stoi(inputArr.at(0));
            if (taskID < 0) {
                throw std::invalid_argument(INVALID_TASK_ID_ARGUMENT);
            }
        } catch (std::invalid_argument e) {
            throw std::invalid_argument(INVALID_TASK_ID_ARGUMENT);
        }
        return SetDoneTaskCommand(taskID-1);
    }

    SetUndoneTaskCommand CommandParser::parseSetUndone(std::vector<std::string> inputArr) {
        if (inputArr.size() == 0) {
            throw std::invalid_argument(TOO_LITTLE_ARGUMENTS_MESSAGE);
        }
        else if (inputArr.size() > 1) {
            throw std::invalid_argument(TOO_MANY_ARGUMENTS_MESSAGE);
        }

        int taskID;
        try {
            taskID = std::stoi(inputArr.at(0));
            if (taskID < 0) {
                throw std::invalid_argument(INVALID_TASK_ID_ARGUMENT);
            }
        }
        catch (std::invalid_argument e) {
            throw std::invalid_argument(INVALID_TASK_ID_ARGUMENT);
        }
        return SetUndoneTaskCommand(taskID-1);
    }

    SearchTaskCommand CommandParser::parseSearch(std::vector<std::string> inputArr) {
        if (inputArr.size() == 0) {
            throw std::invalid_argument(TOO_LITTLE_ARGUMENTS_MESSAGE);
        }

        std::string arg = ParserLibrary::implode(inputArr, " ");
        return SearchTaskCommand(arg);
    }


    EditTaskCommand CommandParser::parseEdit(std::vector<std::string> inputArr) {
        if (inputArr.size() < 2) {
            throw std::invalid_argument(TOO_LITTLE_ARGUMENTS_MESSAGE);
        }

        int taskID;
        try {
            taskID = std::stoi(inputArr.at(0));
        } catch (std::invalid_argument e) {
            throw std::invalid_argument(INVALID_TASK_ID_ARGUMENT);
        }

        std::vector<std::string> subVec(inputArr.begin() + 1, inputArr.end());
        AbstractTask* task = TaskParser::parseTask(subVec);
        return EditTaskCommand(taskID-1, task);
    }


    DeleteTaskCommand CommandParser::parseDelete(std::vector<std::string> inputArr) {
        if (inputArr.size() == 0) {
            throw std::invalid_argument(TOO_LITTLE_ARGUMENTS_MESSAGE);
        } else if (inputArr.size() > 1) {
            throw std::invalid_argument(TOO_MANY_ARGUMENTS_MESSAGE);
        }

        int taskID;
        try {
            taskID = std::stoi(inputArr.at(0));
        } catch (std::invalid_argument e) {
            throw std::invalid_argument(INVALID_TASK_ID_ARGUMENT);
        }
        return DeleteTaskCommand(taskID-1);
    }


    ClearTaskCommand CommandParser::parseClear(std::vector<std::string> inputArr) {
        if (inputArr.size() > 0) {
            throw std::invalid_argument(TOO_MANY_ARGUMENTS_MESSAGE);
        }

        ClearTaskCommand command;
        return command;
    }


    UndoTaskCommand CommandParser::parseUndo(std::vector<std::string> inputArr) {
        if (inputArr.size() > 0) {
            throw std::invalid_argument(TOO_MANY_ARGUMENTS_MESSAGE);
        }

        UndoTaskCommand command;
        return command;
    }


    AbstractCommand* CommandParser::parse(std::string input) {
        std::vector<std::string> inputArr = ParserLibrary::explode(input, " ");
        std::string command = DoLah::CommandTokenizer::findCommand(inputArr);

        if (command.empty()) {
            // default command
            command = ADD.at(0);
        } else {
            inputArr = CommandTokenizer::pruneCommand(inputArr);
        }

        if (ParserLibrary::inStringArray(ADD, command)) {
            AddTaskCommand* command = new AddTaskCommand(parseAdd(inputArr));
            return command;
        } else if (ParserLibrary::inStringArray(SEARCH, command)) {
            SearchTaskCommand* command = new SearchTaskCommand(parseSearch(inputArr));
            return command;
        } else if (ParserLibrary::inStringArray(DONE, command)) {
            SetDoneTaskCommand* command = new SetDoneTaskCommand(parseSetDone(inputArr));
            return command;
        }
        else if (ParserLibrary::inStringArray(UNDONE, command)) {
            SetUndoneTaskCommand* command = new SetUndoneTaskCommand(parseSetUndone(inputArr));
            return command;
        }
        else if (ParserLibrary::inStringArray(EDIT, command)) {
            EditTaskCommand* command = new EditTaskCommand(parseEdit(inputArr));
            return command;
        } else if (ParserLibrary::inStringArray(DELETE, command)) {
            DeleteTaskCommand* command = new DeleteTaskCommand(parseDelete(inputArr));
            return command;
        } else if (ParserLibrary::inStringArray(CLEAR, command)) {
            ClearTaskCommand* command = new ClearTaskCommand(parseClear(inputArr));
            return command;
        } else if (ParserLibrary::inStringArray(UNDO, command)) {
            UndoTaskCommand* command = new UndoTaskCommand(parseUndo(inputArr));
            return command;
        } else {
            throw std::invalid_argument(UNHANDLED_COMMAND_MESSAGE);
        }
    }
}

