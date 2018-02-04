#include "Archive.h"
#include <sys/stat.h>
#include <iostream>
int main() {
    Archive tester = Archive("/home/aleksandr/Downloads","outtest.arc");
    tester.write();
    return 0;
}
