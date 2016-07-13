#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

int main() {
	vector<int> nums(9);

	nums[0] = 1;
	nums[8] = 1;

	for (vector<int>::iterator top = nums.begin(); (top = find(top, nums.end(), 1)) != nums.end(); top++) {
		cout << "Œ©‚Â‚©‚è‚Ü‚µ‚½ " << (top - nums.begin()) << " " << *top << "\n";
	}

	for (vector<int>::iterator top2 = nums.begin(); (top2 = find_if(top2, nums.end(), bind2nd(equal_to<int>(), 1))) != nums.end(); top2++) {
		cout << "Œ©‚Â‚©‚è‚Ü‚µ‚½ " << (top2 - nums.begin()) << " " << *top2 << "\n";
	}
	//vector<int>::iterator it = find_if(nums.begin(), nums.end(), std::bind2nd( std::greater_equal< int >(), 4 ));
	//vector<int>::iterator it = find_if(nums.begin(), nums.end(), bind2nd( not_equal_to< int >(), 9 ));

	//if (it != nums.end()) {
	//	cout << "Œ©‚Â‚©‚è‚Ü‚µ‚½ (" << *it << ")\n";
	//} else {
	//	cout << "Œ©‚Â‚©‚è‚Ü‚¹‚ñ\n";
	//}

	return 0;
}
