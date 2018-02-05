#include "ArchiveEncoder.h"
#include <sys/stat.h>
#include <iostream>
int main() {
    ArchiveEncoder tester = ArchiveEncoder("/home/aleksandr/Downloads","outtest.arc");
    tester.write();
    return 0;
}
