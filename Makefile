EXE = test

all: $(EXE)
	@$(call random_shmol_cat, "Making", "Science ~", $(CLEAR), $(RESET));
	./$(EXE)

SRC := $(shell find . -name '*.cpp')

$(EXE): $(SRC) Log/Log.hpp
	@if ! g++ $(SRC) -o $(EXE); then \
		$(call shmol_cat_error, $(RED), $(RED_L)); \
		exit 1; \
	fi

# --------------------------------------------------------------------------------- >
git: fclean
	@$(call random_shmol_cat, 作業を保存してるかな.., いいね、いいねえー , $(CLS), );
	git add .; \
	git commit; \
	git push

clean:
	@rm -rf log
	@$(call random_shmol_cat, "a bit of", "cleaning ~", , );

fclean: clean
	@rm -f $(EXE)
	@rm -rf log
	@$(call random_shmol_cat, All, "Cleaned ~", , );

.PHONY: 
	all clean fclean git

# --------------------------------------------------------------------------------- >
CLEAR = \033[2J\033[H
RESET = \033[0m
# @$(call random_shmol_cat, text 1, text 2, $(CLS), $(RESET));
define random_shmol_cat
	COLOR=$$(printf "\033[38;5;%dm" $$(shuf -i 1-255 -n 1)); \
	COLOR2=$$(printf "\033[38;5;%dm" $$(shuf -i 0-255 -n 1)); \
	echo -e "$(3)$${COLOR2}\
	\tにゃ~$${COLOR}\t⠀╱|、\n\
	\t\t(˚ˎ。7⠀⠀⠀$${COLOR2}~ $(1) ~$${COLOR}\n\
	\t\t⠀|、˜\\\\\t\t$${COLOR2}~ $(2)$${COLOR}\n\
	\t\t⠀じしˍ)ノ\n$(4)"
endef

# --------------------------------------------------------------------------------- >
RED = \033[38;5;1m
RED_L = \033[38;5;9m
# @$(call shmol_cat_error, $(RED), $(RED_L));
define shmol_cat_error
	echo -e "$(2)\
	\tにゃ~$(1)\t⠀╱|、\n\
	\t\t(˚ˎ。7⠀⠀⠀$(2)~ somshin wen wong ~$(1)\n\
	\t\t⠀|、˜\\\\\n\
	\t\t⠀じしˍ)ノ\n$(RESET)"
endef
