obj-m := devno.o

KVER := ${shell uname -r}
KVER := 3.10.0-229.11.1.el7.x86_64

all: Module.symvers
	make -C /lib/modules/$(KVER)/build M=$(PWD) modules

clean:
	rm -f devno.o devno.ko Module.symvers modules.order devno.mod.*

Module.symvers: /lib/modules/$(KVER)/build/Module.symvers
	cp /lib/modules/$(KVER)/build/Module.symvers ${PWD}/Module.symvers




