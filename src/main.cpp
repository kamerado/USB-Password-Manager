#include <iostream>
#include <memory>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <filesystem>

#include "core/DatabaseManager.h"
#include "core/EncryptionUtil.h"
#include "src/gui/GUI/mainwindow.h"
#include "src/gui/GUI/login.h"
#include "src/gui/GUI/setup.h"

#include <QApplication>

namespace fs = std::filesystem;

DatabaseManager* db;
EncryptionUtil* enc;

void createFile(const char *File) {
  std::fstream fs;
  fs.open(File, std::ios::out);
  fs.close();
}

void deleteFile(const char *File) {
  std::remove(File);
}

void handler(EncryptionUtil* encdec) {
  enc = encdec;
}

int main(int argc, char *argv[])
{    

  db = new DatabaseManager();
  QApplication a(argc, argv);
  MainWindow m;
  if(!fs::exists("db/passwords.enc")) {
    Setup s;
    QWidget::connect(&s, &Setup::sendEnc, handler);
    if (s.exec() == QDialog::Accepted) {
      s.getEnc();
      m.setEnc(enc);
      m.show();
      s.~Setup();
    }
    return a.exec();
  } else {
    Login l;
    if (l.exec() == QDialog::Accepted) {
      l.show();
    }
    return a.exec();
  }
}


// int main() {
    // int input;
    // char password[32];
    // const char FileTXT[] = "../db/test.txt";
    // const char FileDB[] = "../db/test.db";

    // EncryptionUtil* encdec = new EncryptionUtil();
    
    // std::cout << "enter passcode of max 32 chars: " << std::endl;
    // std::cin >> password;

    // std::cout << "Please enter a number between 0-2" << std::endl;
    // std::cin >> input;
    // while(input != 0) {
    //     switch (input)
    //     {
    //     case 1:
    //         createFile(FileDB);
    //         encdec->EncryptFile(FileTXT, FileDB, password);
    //         std::cin >> input;
    //         break;
    //     case 2:
    //         createFile(FileTXT);
    //         encdec->DecryptFile(FileDB, FileTXT, password);
    //         deleteFile(FileDB);
    //         std::cout << "Please enter a number between 0-2" << std::endl;
    //         std::cin >> input;
    //         break;
        
    //     default:
    //         std::cout << "Error: Please enter a number between 0-2" << std::endl;
    //         std::cin >> input;
    //         break;
    //     }
    // }

    // return 0;
    //         deleteFile(FileTXT);
    //         std::cout << "Please enter a number between 0-2" << std::endl;
    //         std::cin >> input;
    //         break;
    //     case 2:
    //         createFile(FileTXT);
    //         encdec->DecryptFile(FileDB, FileTXT, password);
    //         deleteFile(FileDB);
    //         std::cout << "Please enter a number between 0-2" << std::endl;
    //         std::cin >> input;
    //         break;
        
    //     default:
    //         std::cout << "Error: Please enter a number between 0-2" << std::endl;
    //         std::cin >> input;
    //         break;
    //     }
    // }

//     return 0;

// }