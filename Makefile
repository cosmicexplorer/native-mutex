.PHONY: all check clean distclean

NPM_DIR := node_modules

NPM_OBJ_DIR := build
NPM_BIN := $(shell npm bin)
COFFEE_BIN := $(NPM_BIN)/coffee
GYP_BIN := $(NPM_BIN)/node-gyp

CPP_OBJ := $(NPM_OBJ_DIR)/Release/native-mutex.node

TEST_DRIVER := driver.coffee
TEST_CHECKER := checker.coffee
TEST_OBJ := log

all: $(CPP_OBJ)

%.js: %.coffee

$(NPM_DIR):
	npm install

$(CPP_OBJ): $(NPM_DIR)
	$(GYP_BIN) configure
	$(GYP_BIN) build

check: $(TEST_DRIVER) $(TEST_CHECKER) all
	$(COFFEE_BIN) $(TEST_DRIVER) > $(TEST_OBJ)
	$(COFFEE_BIN) $(TEST_CHECKER) < $(TEST_OBJ)

clean:
	rm -rf $(NPM_OBJ_DIR)
	rm -f $(TEST_OBJ)

distclean: clean
	rm -rf $(NPM_DIR)
