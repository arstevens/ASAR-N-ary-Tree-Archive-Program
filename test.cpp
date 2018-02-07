#include "ArchiveEncoder.h"

int main() {
	ArchiveEncoder tester("/home/aleksandr/Downloads","outtest.arc");
	tester.write();
	return 0;
}
