ch			:= "ch"
num_set		:= $(shell seq 01 09)
#modules		:= $(foreach str, 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20, $(ch)$(str))
modules		:= $(foreach str, $(num_set), $(ch)$(str))

.PHONY:	all $(modules)
all: $(modules)
	@echo "$(modules)"

$(modules):
	$(MAKE) --directory=$@

clean:
	for dir in $(modules); do \
		$(MAKE) -C $$dir clean; \
	done
