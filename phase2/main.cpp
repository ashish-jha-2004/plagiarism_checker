#include "plagiarism_checker.hpp"

#include <map>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// You should NOT modify ANYTHING in this file.

void student_t::flag_student(std::shared_ptr<submission_t> __submission) {
    int i = 0;
    while (__submission->codefile[i] != '/') i++;
    i++;
    std::cout << "I was flagged in " << __submission->codefile.substr(i) << 
        " and must defend myself in front of Prof. " <<
        __submission->professor->get_name() << "." << std::endl;
}

void professor_t::flag_professor(std::shared_ptr<submission_t> __submission) {
    int i = 0;
    while (__submission->codefile[i] != '/') i++;
    i++;
    std::cout << "Student " << __submission->student->get_name() << 
        " has plagiarized in " << __submission->codefile.substr(i) << 
        " and will be receiving an FR grade." << std::endl << std::endl;
}

namespace testcase {
    std::string test_dir;
    std::map<std::string, std::shared_ptr<student_t>> students; 
    std::map<std::string, std::shared_ptr<professor_t>> professors; 
    std::vector<std::shared_ptr<submission_t>> pre_existing_codes;
    void get_students(void);
    void get_professors(void);
    void get_pre_existing_codes(void);
    void execute_testcase(std::string test_dir);
}

void testcase::get_students(void) {
    std::string filename = testcase::test_dir + "/students.txt";
    std::ifstream in(filename);
    std::string student_name;
    while (in >> student_name) {
        testcase::students[student_name] = 
            std::make_shared<student_t>(student_name);
    }
    in.close();
}

void testcase::get_professors(void) {
    std::string filename = testcase::test_dir + "/professors.txt";
    std::ifstream in(filename);
    std::string professor_name;
    while (in >> professor_name) {
        testcase::professors[professor_name] = 
            std::make_shared<professor_t>(professor_name);
    }
    in.close();
}

void testcase::get_pre_existing_codes(void) {
    std::ifstream in(testcase::test_dir + "/originals.txt");
    long id;
    std::string student_name;
    std::string professor_name;
    std::string token;
    std::string code_file;
    while (in >> id >> student_name >> professor_name >> code_file) {
        std::shared_ptr<submission_t> submission = 
            std::make_shared<submission_t>();
        submission->id = id;
        submission->student = testcase::students[student_name];
        submission->professor = testcase::professors[professor_name];
        submission->codefile = testcase::test_dir + "/" + code_file + ".cpp";
        testcase::pre_existing_codes.push_back(submission);
    }
    in.close();
}


void testcase::execute_testcase(std::string __test_dir) {
    testcase::test_dir = __test_dir;
    testcase::get_students();
    testcase::get_professors();
    testcase::get_pre_existing_codes();
    plagiarism_checker_t checker(testcase::pre_existing_codes);
    std::ifstream in(testcase::test_dir + "/submissions.txt");
    double timestamp;
    double old_timestamp = 0.0;
    long id;
    std::string student_name;
    std::string prof_name;
    std::string code_file;
    while (in >> timestamp >> id >> student_name >> prof_name >> code_file) {
        usleep((timestamp - old_timestamp) * 1000000);
        std::shared_ptr<submission_t> submission = 
            std::make_shared<submission_t>();
        submission->id = id;
        submission->student = testcase::students[student_name];
        submission->professor = testcase::professors[prof_name];
        submission->codefile = testcase::test_dir + "/" + code_file + ".cpp";
        checker.add_submission(submission);
        old_timestamp = timestamp;
    }
    in.close();
}

int main(int argc, char** argv) {
    testcase::execute_testcase(argv[1]);
    return 0;
}
