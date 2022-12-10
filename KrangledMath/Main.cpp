#include "KRMath\KRMath.h"
#include <string>

int main()
{
	KRM::FVector3 testVec{1, 0, 0};
	KRM::FVector3 test1 = testVec.Cross(testVec);
	KRM::FVector3 test2 = KRM::Cross(testVec, test1);
	return 0;
}