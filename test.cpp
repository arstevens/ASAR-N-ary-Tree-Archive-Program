#include "ArchiveEncoder.h"

int main() {
	ArchiveEncoder tester("/etc","outtest.arc");
	tester.write();
	return 0;
}
