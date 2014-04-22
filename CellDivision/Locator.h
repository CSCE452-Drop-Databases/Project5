#ifndef LOCATOR_H
#define LOCATOR_H

class Locator {
private:
	int next_locator;
public:
	Locator(int locator = 0) : next_locator(0) {}

	int nextLocator() {
		;
		return ++next_locator;
	}

};

#endif
