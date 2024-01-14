#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <algorithm> // Pentru std::find_if
#include <vector>
#include <string>
#include <ctime>
using namespace std;

class FlowStep
{
protected:
    string name;
    string description;
    bool executed;
    bool skipped;
    int errors = 0;

public:
    FlowStep(string Name, string Description) : name(Name), description(Description), executed(false), skipped(false) {}

    string getName()
    {
        return name;
    }
    string getDescription()
    {
        return description;
    }
    virtual void setState(bool Executed) { executed = Executed; }

    virtual bool getState() { return executed; }

    virtual int getError() { return errors; }

    virtual bool getSkipped() { return skipped; }

    virtual void execute() = 0;

    virtual void displayDetails()
    {
        cout << endl;
        cout << "Step name: " << name << "\n";
        cout << "Description: " << description << "\n";
    }

    virtual bool validateInput(string input) { return true; }

    virtual string extractInfo() { return " "; }

    // Functie care ii da utilizatorului posibilitatea de a da skip unei etape
    virtual bool Skip()
    {
        string input;
        cout << "Write yes if you want to skip this step or no if you don't want to skip this step: " << endl;
        while (true)
        {
            getline(cin, input);
            if (input.find_first_not_of(' ') == string::npos)
            {
                cout << "Input should not be empty. Try again: "; // pt cand apasa doar enter
            }
            else if (input == "yes")
            {
                cout << "Step skipped." << endl;
                skipped = true;
                return true;
            }
            else if (input == "no")
            {
                return false;
            }
            else
            {
                cout << "Invalid input. Try again: ";
            }
        }
    }

    virtual void displayProgress() {}

    void ifError(const invalid_argument &e)
    {
        int c;
        cout << "Error: " << e.what() << endl;
        cout << "Choose if you want to reload the step or go to the next one " << endl;
        cout << "\t|    Press 1 to Reload the Step                 |" << endl;
        cout << "\t|    Press 2 to Go to the next Step             |" << endl;
        cin >> c;
        switch (c)
        {
        case 1:
            cin.ignore();
            execute();
            break;
        case 2:
            cin.ignore();
            skipped = true;
            break;
        default:
            system("cls");
            cout << "\t\t\t Please select from the options given above \n"
                 << endl;
        }
    }

    virtual ~FlowStep() = default;
};

class TitleStep : public FlowStep
{
private:
    string title, subtitle;

public:
    TitleStep(string name, string description) : FlowStep(name, description) {}

    bool validateInput(string input) override
    {
        if (input == "" || input.length() < 2 || input.length() > 50 || input.find_first_not_of(' ') == string::npos)
            return false;
        return true;
    }

    void execute() override
    {
        displayDetails();
        if (Skip())
        {
            return;
        }
        else
        {
            try
            {
                cout << "\tEnter the title : " << endl;
                getline(cin, title);
                if (validateInput(title) == false)
                {
                    errors++;
                    throw invalid_argument("Invalid title. Title cannot be empty and must have between 2 and 50 characters.");
                }
                cout << "\tEnter the subtitle : " << endl;
                getline(cin, subtitle);
                if (validateInput(subtitle) == false)
                {
                    errors++;
                    throw invalid_argument("Invalid subtitle. Subtitle cannot be empty and must have between 2 and 50 characters.");
                }

                executed = true;
            }
            catch (const invalid_argument &e)
            {
                title = "";
                subtitle = "";
                ifError(e);
            }
        }
    }

    void displayProgress() override
    {
        time_t now = time(nullptr);
        if (title.empty() && subtitle.empty())
        {
            return;
        }
        cout << "TitleStep completed with title: " << title << " and subtitle: " << subtitle << endl;
        cout << "Number of error screens displayed: " << errors << endl;
        cout << "Completion time: " << asctime(localtime(&now)) << endl;
    }

    string extractInfo() override
    {
        if (title.empty() && subtitle.empty())
        {
            return "Title and subtitle are empty.";
        }
        return title + " " + subtitle;
    }
};

class TextStep : public FlowStep
{
private:
    string title, copy;

public:
    TextStep(string name, string description) : FlowStep(name, description) {}

    bool validateInput(string input) override
    {
        if (input == "" || input.length() < 2 || input.find_first_not_of(' ') == string::npos)
            return false;
        return true;
    }

    void execute() override
    {
        displayDetails();
        if (Skip())
        {
            return;
        }
        else
        {
            try
            {

                cout << "\tEnter the title : " << endl;
                getline(cin, title);
                if (validateInput(title) == false || title.length() > 50)
                {
                    errors++;
                    throw invalid_argument("Invalid title. Title cannot be empty and must have between 2 and 50 characters.");
                }

                cout << "\tEnter text : " << endl;
                getline(cin, copy);
                if (validateInput(copy) == false || copy.length() > 100)
                {
                    errors++;
                    throw invalid_argument("Invalid text. Text cannot be empty and must have between 2 and 100 characters.");
                }
                executed = true;
            }
            catch (const invalid_argument &e)
            {
                title = "";
                copy = "";
                ifError(e);
            }
        }
    }

    string extractInfo() override
    {
        if (title.empty() && copy.empty())
        {
            return "Title and copy are empty.";
        }
        return title + "\n" + copy;
    }

    void displayProgress() override
    {
        if (title.empty() && copy.empty())
        {
            return;
        }
        time_t now = time(nullptr);
        cout << "TextStep completed with title: " << title << " and copy: " << copy << endl;
        cout << "Number of error screens displayed: " << errors << endl;
        cout << "Completion time: " << asctime(localtime(&now)) << endl;
    }
};

class TextInputStep : public FlowStep
{
private:
    string desc, text_input;

public:
    TextInputStep(string name, string description) : FlowStep(name, description) {}

    bool validateInput(string input) override
    {
        if (input == "" || input.length() < 2 || input.find_first_not_of(' ') == string::npos)
            return false;
        return true;
    }

    void execute() override
    {
        displayDetails();
        if (Skip())
        {
            return;
        }
        else
        {
            try
            {
                cout << "\tEnter text : " << endl;
                getline(cin, text_input);
                if (validateInput(text_input) == false || text_input.length() > 50)
                {
                    errors++;
                    throw invalid_argument("Invalid text. Text cannot be empty and must have between 2 and 50 characters.");
                }
                cout << "\tEnter description : " << endl;
                getline(cin, desc);
                if (validateInput(desc) == false || desc.length() > 200)
                {
                    errors++;
                    throw invalid_argument("Invalid description. Description cannot be empty and must have between 2 and 200 characters.");
                }
                executed = true;
            }
            catch (const invalid_argument &e)
            {
                desc = "";
                text_input = "";
                ifError(e);
            }
        }
    }

    string extractInfo() override
    {
        if (desc.empty() && text_input.empty())
        {
            return "Text input and description are empty.";
        }
        return text_input + "\n" + desc; // sau return text_input ;
    }

    void displayProgress() override
    {
        if (desc.empty() && text_input.empty())
        {
            return;
        }
        time_t now = time(nullptr);
        cout << "TextInputStep completed with textInput: " << text_input << " and description: " << desc << endl;
        cout << "Number of error screens displayed: " << errors << endl;
        cout << "Completion time: " << asctime(localtime(&now)) << endl;
    }
};

class NumberInputStep : public FlowStep
{
private:
    string desc;
    float number = 0;

public:
    NumberInputStep(string name, string description) : FlowStep(name, description) {}

    float getNumber()
    {
        return number;
    }
    void setNumber(float Number)
    {
        number = Number;
    }
    void setDescription(string s)
    {
        desc = s;
    }

    // template <typename T>
    bool validateInput(string input)
    {
        if (input == "" || input.length() > 200 || input.length() < 2)
            return false;
        return true;
    }
    bool validateInput2(string input)
    {
        for (char ch : input)
        {
            if (!isdigit(ch) && ch != '.')
                return false;
        }
        return true;
    }

    void execute() override
    {
        displayDetails();
        if (Skip())
        {
            return;
        }
        else
        {
            try
            {
                string input;
                cout << "\tEnter number:  " << endl;
                getline(cin, input);

                if (validateInput2(input) == false)
                {
                    errors++;
                    throw invalid_argument("Invalid number.");
                }
                number = stof(input);

                cout << "\tEnter description : " << endl;
                getline(cin, desc);
                if (validateInput(desc) == false)
                {
                    errors++;
                    throw invalid_argument("Invalid description. Description cannot be empty and must have between 2 and 200 characters.");
                }
                executed = true;
            }
            catch (const invalid_argument &e)
            {
                desc = "";
                number = 0;
                ifError(e);
            }
        }
    }

    string extractInfo() override
    {
        if (desc.empty() && number == 0)
        {
            return "Title and subtitle are empty.";
        }
        return to_string(number);
    }

    void displayProgress() override
    {
        if (desc.empty() && number == 0)
        {
            return;
        }
        time_t now = time(nullptr);
        cout << "NumberInputStep completed with input number: " << number << " and input description: " << desc << endl;
        cout << "Number of error screens displayed: " << errors << endl;
        cout << "Completion time: " << asctime(localtime(&now)) << endl;
    }
};

class CalculusStep : public FlowStep
{
private:
    NumberInputStep number1{"First Number Input Step", "Description for first number"};
    NumberInputStep number2{"Second Number Input Step", "Description for second number"};
    string operation;
    float result = 0.0f;

public:
    CalculusStep(string name, string description) : FlowStep(name, description) {}

    void execute() override
    {
        displayDetails();
        if (Skip())
        {
            return;
        }
        else
        {
            try
            {
                cout << "First number: " << endl;
                number1.execute();
                cout << "Second number: " << endl;
                number2.execute();

                cout << "Enter operation (+, -, *, /, min, max): ";
                cin >> operation;

                if (operation == "+")
                {
                    result = number1.getNumber() + number2.getNumber();
                }
                else if (operation == "-")
                {
                    result = number1.getNumber() - number2.getNumber();
                }
                else if (operation == "*")
                {
                    result = number1.getNumber() * number2.getNumber();
                }
                else if (operation == "/")
                {
                    if (number2.getNumber() != 0)
                    {
                        result = number1.getNumber() / number2.getNumber();
                    }
                    else
                    {
                        errors++;
                        throw invalid_argument("Division by zero is not allowed.");
                    }
                }
                else if (operation == "min")
                {
                    result = min(number1.getNumber(), number2.getNumber());
                }
                else if (operation == "max")
                {
                    result = max(number1.getNumber(), number2.getNumber());
                }
                else
                {
                    errors++;
                    throw invalid_argument("Invalid operation.");
                }
                cout << "Result of operation: " << result << endl;
                cin.ignore();
            }
            catch (const invalid_argument &e)
            {
                result = 0.0f;
                number1.setNumber(0);
                number2.setNumber(0);
                number1.setDescription("");
                number2.setDescription("");
                ifError(e);
            }
        }
    }

    string extractInfo() override
    {
        if (result == 0.0f)
        {
            return "Result is empty.";
        }
        return "Number1 = " + to_string(number1.getNumber()) + ", Number2 = " + to_string(number2.getNumber()) + ", Operation = " + operation + ", Result = " + to_string(result);
    }

    void displayProgress() override
    {
        if (result == 0.0f)
        {
            return;
        }
        time_t now = time(nullptr);
        cout << "Calculus Step is completed." << endl;
        cout << "Number of error screens displayed: " << errors << endl;
        cout << "Completion time: " << asctime(localtime(&now)) << endl;
    }
};

class TextFileInputStep : public FlowStep
{
private:
    string fileDescription, fileName;

public:
    TextFileInputStep(string name, string description) : FlowStep(name, description) {}

    string getFileName()
    {
        return fileName;
    }

    bool validateInput(string fName) override
    {
        if (fName.substr(fName.find_last_of(".") + 1) == "txt") // fileName.find_last_of(".") determină ultima poziție a caracterului .
            return true;
        return false;
    }

    void execute() override
    {
        displayDetails();
        if (Skip())
        {
            return;
        }
        else
        {
            try
            {
                cout << "\tEnter file description : " << endl;
                getline(cin, fileDescription);
                if (fileDescription == "" || fileDescription.length() < 2 || fileDescription.length() > 500)
                {
                    errors++;
                    throw invalid_argument("Invalid file description. Description cannot be empty.");
                }
                cout << "\tEnter file name:  " << endl;
                getline(cin, fileName);
                if (validateInput(fileName) == false)
                {
                    errors++;
                    throw invalid_argument("Invalid file name. The file name must contain the .txt extension.");
                }
                executed = true;
            }
            catch (const invalid_argument &e)
            {
                fileName = "";
                fileDescription = "";
                ifError(e);
            }
        }
    }

    string extractInfo() override
    {
        if (fileName.empty() && fileDescription.empty())
        {
            return "File name and file description are empty.";
        }
        return "Description = " + fileDescription + ", File Name = " + fileName;
    }

    void displayProgress() override
    {
        if (fileName.empty() && fileDescription.empty())
        {
            return;
        }
        time_t now = time(nullptr);
        cout << "Text file input step completed." << endl;
        cout << "Number of error screens displayed: " << errors << endl;
        cout << "Completion time: " << asctime(localtime(&now)) << endl;
    }
};

class CsvFileInputStep : public FlowStep
{
private:
    string fileDescription, fileName;

public:
    CsvFileInputStep(string name, string description) : FlowStep(name, description) {}

    string getFileName()
    {
        return fileName;
    }

    bool validateInput(string fName) override
    {
        if (fName.substr(fName.find_last_of(".") + 1) == "csv") // fileName.find_last_of(".") determină ultima poziție a caracterului .
            return true;
        return false;
    }

    void execute() override
    {
        displayDetails();
        if (Skip())
        {
            return;
        }
        else
        {
            try
            {
                cout << "\tEnter file description : " << endl;
                getline(cin, fileDescription);
                if (fileDescription == "" || fileDescription.length() < 2 || fileDescription.length() > 500)
                {
                    errors++;
                    throw invalid_argument("Invalid file description. Description cannot be empty.");
                }
                cout << "\tEnter file name:  " << endl;
                getline(cin, fileName);
                if (validateInput(fileName) == false)
                {
                    errors++;
                    throw invalid_argument("Invalid file name. The file name must contain the .csv extension.");
                }
                executed = true;
            }
            catch (const invalid_argument &e)
            {
                fileDescription = "";
                fileName = "";
                ifError(e);
            }
        }
    }

    string extractInfo() override
    {
        if (fileName.empty() && fileDescription.empty())
        {
            return "File name and file description are empty.";
        }
        return "Description = " + fileDescription + ", File Name = " + fileName;
    }

    void displayProgress() override
    {
        if (fileName.empty() && fileDescription.empty())
        {
            return;
        }
        time_t now = time(nullptr);
        cout << "Csv file input step completed." << endl;
        cout << "Number of error screens displayed: " << errors << endl;
        cout << "Completion time: " << asctime(localtime(&now)) << endl;
    }
};

class DisplaySteps : public FlowStep
{
private:
    int step;
    FlowStep *previousStep;
    TextFileInputStep *textInputStep;
    CsvFileInputStep *csvInputStep;

public:
    DisplaySteps(string name, string description, TextFileInputStep *TextInputStep, CsvFileInputStep *CsvInputStep) : FlowStep(name, description), textInputStep(TextInputStep), csvInputStep(CsvInputStep) {}

    void selectPreviousStep()
    {
        cout << "Choose file type to read (txt/csv): ";
        string file_type;
        cin >> file_type;

        if (file_type == "txt")
        {
            if (textInputStep->getSkipped() == false)
            {
                previousStep = textInputStep;
                step = 6;
            }
        }
        else if (file_type == "csv")
        {
            if (csvInputStep->getSkipped() == false)
            {
                previousStep = csvInputStep;
                step = 7;
            }
        }
        else
        {
            cout << "Invalid file type selected. Try again." << endl;
            errors++;
            selectPreviousStep();
        }
    }

    void execute() override
    {
        displayDetails();
        if (Skip())
        {
            return;
        }
        else
        {
            selectPreviousStep();

            if (previousStep != nullptr)
            {
                if (step == 6)
                    readFromTextFile(textInputStep->getFileName());
                else
                    readFromCsvFile(csvInputStep->getFileName());
            }
            else
            {
                cout << "No previous step provided." << endl;
            }
        }
    }

    void readFromTextFile(string fileName)
    {
        ifstream file(fileName);
        string line;
        if (!file.is_open())
        {
            cout << "Error opening the text file " << fileName << endl;
            errors++;
            return;
        }
        cout << "Content of Text File:" << endl;
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }

    void readFromCsvFile(string fileName)
    {
        ifstream file(fileName);
        string line;
        if (!file.is_open())
        {
            cout << "Error opening the csv file " << fileName << endl;
            errors++;
            return;
        }
        cout << "Content of CSV File:" << endl;
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }

    string extractInfo() override
    {
        TextFileInputStep *textFileStep = dynamic_cast<TextFileInputStep *>(previousStep);
        CsvFileInputStep *csvFileStep = dynamic_cast<CsvFileInputStep *>(previousStep);
        string fileName;
        if (step == 6)
        {
            fileName = textFileStep->getFileName();
        }
        else
        {
            fileName = csvFileStep->getFileName();
        }

        ifstream file(fileName);
        if (!file.is_open())
        {
            cout << "Error opening the text file " << fileName << endl;
            errors++;
            return "";
        }
        // Utilizarea stringstream pentru a citi întregul conținut
        stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();
        file.close();
        return content;
    }

    void displayProgress() override
    {
        if (previousStep != nullptr)
        {
            time_t now = time(nullptr);
            cout << "Display step completed." << endl;
            cout << "Number of error screens displayed: " << errors << endl;
            cout << "Completion time: " << asctime(localtime(&now)) << endl;
        }
    }
};

class OutputStep : public FlowStep
{
private:
    string nameOfFile;
    string title;
    string desc;
    int step;

public:
    OutputStep(string name, string description) : FlowStep(name, description) {}

    bool validateInput(string input) override
    {
        if (input == "")
            return false;
        return true;
    }

    void execute()
    {
        try
        {
            cout << "\tEnter the Name of the File : " << endl;
            getline(cin, nameOfFile);
            if (validateInput(nameOfFile) == false)
            {
                errors++;
                throw invalid_argument("Invalid name of file. Name cannot be empty.");
            }
            cout << "\tEnter the Title of the File : " << endl;
            getline(cin, title);
            if (validateInput(title) == false || title.length() < 2 || title.length() > 50)
            {
                errors++;
                throw invalid_argument("Invalid title. Title cannot be empty.");
            }
            cout << "\tEnter the Description of the File : " << endl;
            getline(cin, desc);
            if (validateInput(desc) == false || desc.length() < 5 || desc.length() > 300)
            {
                errors++;
                throw invalid_argument("Invalid description. Description cannot be empty.");
            }
        }
        catch (const invalid_argument &e)
        {
            nameOfFile = "";
            title = "";
            desc = "";
            ifError(e);
        }
    }
    void executeStep(vector<FlowStep *> previousSteps)
    {
        displayDetails();
        if (Skip())
        {
            return;
        }
        else
        {
            execute();
            selectPreviousStep(previousSteps);
        }
    }

    void selectPreviousStep(vector<FlowStep *> previousSteps)
    {
        string answer;
        cout << "Do you want to add information from the previous steps to the file? ";
        cout << "Choose yes / no ";
        cin >> answer;
        if (answer == "no")
            cout << "The generated text file is: " << nameOfFile << " with title: " << title << " and description: " << description << endl;
        else if (answer == "yes")
        {
            cout << "Steps you can choose from: " << endl;
            int i = 1;
            for (auto &step : previousSteps)
            {
                cout << i << ". " << step->getName() << endl;
                i++;
            }
            cout << "\n\t\t\t Please enter your choice : ";
            cin >> step;
            cout << endl;
            if (step >= 1 && step <= previousSteps.size())
            {
                generateOutputFile(previousSteps[step]);
                selectPreviousStep(previousSteps);
            }
            else
            {
                cout << "Invalid choice. Please choose from the options above." << endl;
                errors++;
                selectPreviousStep(previousSteps);
            }
        }
        else
        {
            cout << "Invalid answer. Try again." << endl;
            errors++;
            selectPreviousStep(previousSteps);
        }
    }

    void generateOutputFile(FlowStep *step)
    {
        ofstream outputFile(nameOfFile + ".txt", ios::out | ios::app);
        if (outputFile.is_open())
        {
            outputFile << step->extractInfo() << "\n";
            outputFile.close();
        }
        else
        {
            errors++;
            cout << "Error opening output file. " << endl;
        }
    }

    void displayProgress() override
    {
        time_t now = time(nullptr);
        cout << "OutputStep completed. File generated." << endl;
        cout << "File Name: " << nameOfFile << endl;
        cout << "File Title: " << title << endl;
        cout << "File Description: " << desc << endl;
        cout << "Number of error screens displayed: " << errors << endl;
        cout << "Completion time: " << asctime(localtime(&now)) << endl;
    }
    string extractInfo()
    {
        return "Description = " + desc + ", File Name = " + nameOfFile + ", Title = " + title;
    }
};

class EndStep : public FlowStep
{
public:
    EndStep(string name, string description) : FlowStep(name, description) {}

    void execute()
    {
        cout << "End of flow" << endl;
    }
    void displayProgress() override
    {
        time_t now = time(nullptr);
        cout << "EndStep completed." << endl;
        cout << "Completion time: " << asctime(localtime(&now)) << endl;
    }
};

class FlowBuilder
{
private:
    string name;
    vector<FlowStep *> steps;
    int timesStarted = 0;
    int timesCompleted = 0;
    int NrScreenSkipped = 0;
    int TotalErrors = 0;

public:
    bool validateInput(string input)
    {
        if (input == "")
            return false;
        return true;
    }
    string getName() const
    {
        return name;
    }
    vector<FlowStep *> execute()
    {
        int correct = 0;
        while (correct == 0)
        {
            cin.ignore();
            cout << "\tEnter flow name:  " << endl;
            getline(cin, name);
            if (!validateInput(name))
            {
                cout << "Error: Invalid flow name. The name cannot be empty." << endl;
            }
            else
                correct = 1;
        }

        cout << "\t\t\t___________________________________________\n\n\n";
        cout << "\t\t\t_______________    STEPS    ___________________\n\n\n";
        cout << "\t| TITLE Step                  |" << endl;
        cout << "\t| TEXT Step                   |" << endl;
        cout << "\t| TEXT INPUT Step             |" << endl;
        cout << "\t| NUMBER INPUT Step           |" << endl;
        cout << "\t| CALCULUS Step               |" << endl;
        cout << "\t| TEXT FILE Input Step        |" << endl;
        cout << "\t| CSV FILE Input Step         |" << endl;
        cout << "\t| DISPLAY Steps               |" << endl;
        cout << "\t| OUTPUT Step                 |" << endl;
        cout << "\t| END Step                    |" << endl;
        cout << "                                                     \n\n\n";
        TitleStep *titleStep = new TitleStep("Title Step", "At this step you have to add a title and a subtitle.");
        steps.push_back(titleStep);
        TextStep *textStep = new TextStep("Text Step", "At this step you have to add a title and a copy (text).");
        steps.push_back(textStep);
        TextInputStep *textInputStep = new TextInputStep("Text Input Step", "At this step you must add a text entry and a description of the expected entry.");
        steps.push_back(textInputStep);
        NumberInputStep *numberInputStep = new NumberInputStep("Number Input Step", "At this step you must add an entry number and a description of the expected entry.");
        steps.push_back(numberInputStep);
        CalculusStep *calculusStep = new CalculusStep("Calculus Step", "At this step you must add previous INPUT NUMBER steps and operation symbols, the operation will be performed and the result will be displayed.");
        steps.push_back(calculusStep);
        TextFileInputStep *textFileStep = new TextFileInputStep("Text File Input Step", "At this step you can add .txt files.");
        steps.push_back(textFileStep);
        CsvFileInputStep *csvFileStep = new CsvFileInputStep("Csv File Input Step", "At this step you can add .csv files.");
        steps.push_back(csvFileStep);
        DisplaySteps *displayStep = new DisplaySteps("Display Step", "At this step you can provide as input a previous step that contains information: TEXT INPUT step or CSV INPUT step and you will be able to see the content of the file.", textFileStep, csvFileStep);
        steps.push_back(displayStep);
        OutputStep *outputStep = new OutputStep("Output Step", "At this step you can generate a text file as a result, but you must provide a name, a title, a description for the file that will be generated and you can add information from the previous steps");
        EndStep *endStep = new EndStep("End Step", "At this step you can signal the end of a flux.");
        int k = 1;
        vector<FlowStep *> Allsteps; // toti pasii si cu aia care se repeta
        timesStarted++;
        string answer;
        for (auto &step : steps)
        {
            // int nrErrors = 0;
            step->execute();
            if (step->getSkipped() == false)
            {
                Allsteps.push_back(step);
                while (k == 1)
                {
                    cin.ignore();
                    cout << "Do you want to add this step one more time? \n";
                    cout << "Choose yes / no ";
                    getline(cin, answer);
                    if (answer == "yes")
                    {
                        step->execute();
                        Allsteps.push_back(step);
                    }
                    else if (answer == "no")
                    {
                        k = 0;
                    }
                    else
                    {
                        cout << "Invalid answer. Try again." << endl;
                    }
                }
                k = 1;
            }
        }
        cout << endl;
        outputStep->executeStep(Allsteps);
        if (outputStep->getSkipped() == false)
        {
            Allsteps.push_back(outputStep);
            k = 1;
            while (k == 1)
            {
                cin.ignore();
                cout << "Do you want to add this step one more time? \n";
                cout << "Choose yes / no ";
                getline(cin, answer);
                if (answer == "yes")
                {
                    outputStep->executeStep(Allsteps);
                    Allsteps.push_back(outputStep);
                }
                else if (answer == "no")
                {
                    k = 0;
                }
                else
                {
                    cout << "Invalid answer. Try again." << endl;
                }
            }
        }
        endStep->execute();
        endStep->displayProgress();
        return Allsteps;
    }

    void runflow(vector<FlowStep *> allSteps)
    {
        timesStarted++;
        cout << "Flow '" << name << "' started." << endl;
        int choose;
        for (auto &step : allSteps)
        {
            int obs = 1;
            TotalErrors = TotalErrors + step->getError();
            cout << "Step: " << step->getName() << endl;
            cout << "Choose: " << endl
                 << "1. You complete the step action and select the next one " << endl
                 << "2. Skip the step" << endl;
            while (obs == 1)
            {
                cin >> choose;
                if (choose == 1)
                {
                    step->displayProgress();
                    obs = 0;
                }
                else if (choose == 2)
                {
                    obs = 0;
                    NrScreenSkipped++;
                }
                else
                {
                    cout << "Invalid answer. Try again." << endl;
                    obs = 1;
                }
            }
        }
        cout << "Flow '" << name << "' completed." << endl;
        timesCompleted++;

        cout << "Times Started: " << timesStarted << endl;
        cout << "Times Completed: " << timesCompleted << endl;
        cout << "Number of screens skipped: " << NrScreenSkipped << endl;
        cout << "Mean of errors: " << TotalErrors / allSteps.size() << endl;
    }
};

class FlowManager
{
private:
    vector<FlowBuilder> flows;

public:
    void deleteFlow()
    {
        string flowName;
        cout << "Please enter the name of the flow you want to delete: " << endl;
        cout << "Name: " << endl;
        cin >> flowName;
        auto found = find_if(flows.begin(), flows.end(), [flowName](const FlowBuilder &flow)
                             { return flow.getName() == flowName; });

        if (found != flows.end())
        {
            flows.erase(found);
            cout << "Flow '" << flowName << "' deleted." << endl;
        }
        else
        {
            cout << "Flow '" << flowName << "' not found." << endl;
        }
    }

    void runFlow(vector<FlowStep *> allSteps)
    {
        string flowName;
        cout << "Please enter the name of the flow you want to run: " << endl;
        cout << "Name: " << endl;
        cin >> flowName;
        auto found = find_if(flows.begin(), flows.end(), [&flowName](const FlowBuilder &flow)
                             { return flow.getName() == flowName; });

        if (found != flows.end())
        {
            found->runflow(allSteps);
        }
        else
        {
            cout << "Flow '" << flowName << "' not found." << endl;
        }
    }

    void interface()
    {
        FlowBuilder flow;
        int choice, k = 1;
        string answer;
        vector<FlowStep *> allSteps;

        cout << "\t\t\t\t______________________________________\n";
        cout << "\t\t\t\t                                      \n";
        cout << "\t\t\t\t              Welcome, user!          \n";
        cout << "\t\t\t\t                                      \n";
        cout << "\t\t\t\t______________________________________\n";
        cout << "\t\t\t\t                                      \n";
        cout << "\t\t\t\t|    1) Create flow       |\n";
        cout << "\t\t\t\t|                         |\n";
        cout << "\t\t\t\t|    2) Delete flow       |\n";
        cout << "\t\t\t\t|                         |\n";
        cout << "\t\t\t\t|    3) Run flow          |\n";
        cout << "\t\t\t\t|                         |\n";

        while (k == 1)
        {
            cin.ignore();
            cout << "Do you want to run, create or delete flows? \n";
            cout << "Choose yes / no ";
            getline(cin, answer);
            if (answer == "yes")
            {
                cout << "\n\t\t\t Please select ";
                cin >> choice;
                switch (choice)
                {
                case 1:
                    cin.ignore();
                    allSteps = flow.execute();

                    flows.push_back(flow);
                    break;
                case 2:
                    cin.ignore();
                    deleteFlow();
                    break;
                case 3:
                    cin.ignore();
                    runFlow(allSteps);
                    break;
                default:
                    cout << "\t\t\t Please select from the options given above \n"
                         << endl;
                }
            }
            else if (answer == "no")
            {
                k = 0;
            }

            else
            {
                cout << "Invalid answer. Try again." << endl;
            }
        }
    }
};

int main()
{
    FlowManager flow;
    flow.interface();

    return 0;
}