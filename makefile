CC = gcc

INC_DIR = ./inc
OUT_DIR = ./out
SRC_DIR = ./src

CFLAGS = -Wall -Werror -Wpedantic -std=c99 -Wextra
DEPFLAGS = -MT $@ -MMD -MP -MF $(OUT_DIR)/$*.d

INCLUDE_DBUS = -I/usr/include/dbus-1.0 \
				-I/usr/lib/x86_64-linux-gnu/dbus-1.0/include \
				-I/usr/include/glib-2.0 \
				-I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ \
				-ldbus-1 \
				-ldbus-glib-1 \
				-lglib-2.0 \
				-lgio-2.0 \
				-lgobject-2.0 \
				-lgthread-2.0

INCLUDE_MY = -I$(INC_DIR)

# $(shell mkdir $(OUT_DIR))

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)
DEP = $(SRC:$(SRC_DIR)/%.c=$(OUT_DIR)/%.d)

OUT_START_DBUS 		= $(OUT_DIR)/add_proc_dbus.o $(OUT_DIR)/dbus_functions.o $(OUT_DIR)/read_data.o
OUT_WORK_FILE 		= $(OUT_DIR)/work_file.o $(OUT_DIR)/dbus_functions.o $(OUT_DIR)/read_data.o
OUT_REGISTER_APP 	= $(OUT_DIR)/write_data.o $(OUT_DIR)/register_new.o

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $< $(INCLUDE_MY) $(INCLUDE_DBUS)

register_new_app.exe: $(OUT_REGISTER_APP)
	$(CC) $(CFLAGS) -o $@ $^

start_dbus.exe: $(OUT_START_DBUS)
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE_DBUS)

work_file.exe: $(OUT_WORK_FILE)
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE_DBUS)

clean_out:
	rm -rf $(OUT_DIR)

clean_exe:
	rm *.exe

clean_all:
	clean_out clean_exe

reg:
	./register_new_app.exe

start:
	./start_dbus.exe &

all: register_new_app.exe start_dbus.exe work_file.exe

.PHONY: clean_all clean_out clean_exe reg start all

-include $(DEP)