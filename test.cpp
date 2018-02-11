#include "ArchiveEncoder.h"

int main() {
	ArchiveEncoder tester("testdir","outtest.arc");
	tester.write();
	return 0;
}
