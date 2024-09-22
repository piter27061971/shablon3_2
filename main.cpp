#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Observer {
public:
    virtual void warning(const std::string& message) {};
    virtual void error(const std::string& message) {};
    virtual void fatalError(const std::string& message) {};
protected:
    std::string message_;
};

class WarningObserver : public Observer {
public:
    virtual void warning(const std::string& message) override {
        this->message_ = message;
        std::cout << "WARNING: " << this->message_ << std::endl;
    }
};

class ErrorObserver : public Observer {
public:
    ErrorObserver(std::ofstream& file) : file_(file) {};

    virtual void error(const std::string& message) override {
        this->message_ = message;
        if (file_.is_open()) {
            file_ << "ERROR: " << this->message_ << std::endl;
        }
        else
            std::cout << "File not open" << std::endl;
    }
private:
    std::ofstream& file_;
};

class FatalErrorObserver : public Observer {
public:
    FatalErrorObserver(std::ofstream& file) : file_(file) {};

    virtual void fatalError(const std::string& message) override {
        this->message_ = message;
        std::cout << "FATAL ERROR: " << this->message_ << std::endl;
        if (file_.is_open()) {
            file_ << "FATAL ERROR: " << this->message_ << std::endl;
        }
        else
            std::cout << "File not open" << std::endl;
    }
private:
    std::ofstream& file_;
};

class Subject {
public:
    void addObserver(Observer& observer) {
        observers_.push_back(&observer);
    }
    void removeObserver(Observer* subscriber) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), subscriber), observers_.end());
    }
    void warning(const std::string& message) const {
        for (auto& sub : observers_) {
            sub->warning(message);
        }
    }
    void error(const std::string& message) const {
        for (auto& sub : observers_) {
            sub->error(message);
        }
    }
    void fatalError(const std::string& message) const {
        for (auto& sub : observers_) {
            sub->fatalError(message);
        }
    }
private:
    std::vector<Observer*> observers_;
};


int main(int argc, char* argv[]) {
    std::ofstream file("file.txt");
    const std::string message = "This is error message!";
    Subject subject;

    WarningObserver warningObserver;
    ErrorObserver errorObserver(file);
    FatalErrorObserver fatalObserver(file);

    subject.addObserver(warningObserver);
    subject.warning(message);
    subject.addObserver(errorObserver);
    subject.error(message);
    subject.addObserver(fatalObserver);
    subject.fatalError(message);

    file.close();
    return 0;
}