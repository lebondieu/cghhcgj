include apps/shared/Makefile
include apps/home/Makefile
include apps/on_boarding/Makefile
include apps/hardware_test/Makefile
include apps/usb/Makefile

define ITERATE
$(eval ITERATE_COUNT := 0)\
$(if $(filter ${1},0),,\
  $(call ITERATE_DO,${1})\
)
$(eval ITERATE_COUNT += ${1})
$(filter-out 0, $(ITERATE_COUNT))
endef

define ITERATE_DO
$(if $(word ${1}, ${ITERATE_COUNT}),,\
  $(eval ITERATE_COUNT+=$(words ${ITERATE_COUNT}))\
  $(call ITERATE_DO,${1})\
)
endef

apps =
# All selected apps are included. Each Makefile below is responsible for setting
# the $apps variable (name of the app class) and the $app_headers
# (path to the apps header).
$(foreach i,${apps_list},${eval include apps/$(i)/Makefile})


index_list = $(call ITERATE, $(words $(apps_list)))

apps_can_be_hide = $(foreach x,$(index_list),$(if $(filter $(apps_can_be_hide_list), $(word $(x), $(apps_list))), $(word $(x), $(apps))))

apps_src += $(addprefix apps/,\
  apps_container.cpp \
  apps_container_launch_default.cpp:-onboarding \
  apps_container_launch_on_boarding.cpp:+onboarding \
  apps_container_prompt_beta.cpp:+beta \
  apps_container_prompt_none.cpp:-beta \
  apps_container_prompt_none.cpp:-update \
  apps_container_prompt_update.cpp:+update \
  apps_container_storage.cpp \
  apps_window.cpp \
  backlight_dimming_timer.cpp \
  battery_timer.cpp \
  battery_view.cpp \
  empty_battery_window.cpp \
  exam_pop_up_controller.cpp \
  exam_mode_configuration_official.cpp:+official \
  exam_mode_configuration_non_official.cpp:-official \
  global_preferences.cpp \
  i18n.py \
  lock_view.cpp \
  main.cpp \
  math_toolbox.cpp \
  shift_alpha_lock_view.cpp \
  suspend_timer.cpp \
  title_bar_view.cpp \
  variable_box_controller.cpp \
  variable_box_empty_controller.cpp \
)

null  :=
space := $(null) #
comma := ,

tests_src += apps/exam_mode_configuration_non_official.cpp

snapshots_declaration = $(foreach i,$(apps),$(i)::Snapshot m_snapshot$(subst :,,$(i))Snapshot;)
apps_declaration = $(foreach i,$(apps),$(i) m_$(subst :,,$(i));)
snapshots_construction = $(foreach i,$(apps),,m_snapshot$(subst :,,$(i))Snapshot())
snapshots_list = $(foreach i,$(apps),,&m_snapshot$(subst :,,$(i))Snapshot)
snapshots_count = $(words $(apps))
snapshot_includes = $(foreach i,$(app_headers),-include $(i) )
epsilon_app_names = '$(foreach i,${apps_list},"$(i)", )'
apps_path = $(words $(apps))
apps_can_be_hide_count = $(words $(apps_can_be_hide_list))
apps_can_be_hide_list_declaration = $(subst $(space),$(comma),$(strip $(foreach i, ${apps_list}, $(if $(filter $(apps_can_be_hide_list), $(i)),true,false))))
apps_can_be_hide_settings_message_tree_list = $(subst $(space),$(comma),$(foreach i,$(apps_can_be_hide),SettingsMessageTree(I18n::Message::$(subst :,,$(i)))))
# $(info $(shell $(PYTHON) apps/apps_list_utils.py "create_macros" "$(apps_list)" "$(apps_list_can_be_hide)"))
# $(info apps_list)
# $(info $(apps_list))
# $(info apps_can_be_hide_list)
# $(info $(apps_can_be_hide_list))
# $(info snapshots_declaration)
# $(info $(snapshots_declaration))
# $(info apps_declaration)
# $(info $(apps_declaration))
# $(info snapshots_construction)
# $(info $(snapshots_construction))
# $(info snapshots_list)
# $(info $(snapshots_list))
# $(info snapshots_count)
# $(info $(snapshots_count))
# $(info snapshot_includes)
# $(info $(snapshot_includes))
# $(info epsilon_app_names)
# $(info $(epsilon_app_names))
# $(info apps_can_be_hide_count)
# $(info $(apps_can_be_hide_count))
# $(info apps_can_be_hide_settings_message_tree_list)
# $(info $(apps_can_be_hide_settings_message_tree_list))
# $(info apps_can_be_hide_list_declaration)
# $(info $(apps_can_be_hide_list_declaration))
# $(info )
# $(info )
# $(info Line:)
$(info $(snapshot_includes) -DAPPS_CONTAINER_APPS_DECLARATION="$(apps_declaration)" -DAPPS_CONTAINER_SNAPSHOT_DECLARATIONS="$(snapshots_declaration)" -DAPPS_CONTAINER_SNAPSHOT_CONSTRUCTORS="$(snapshots_construction)" -DAPPS_CONTAINER_SNAPSHOT_LIST="$(snapshots_list)" -DAPPS_CONTAINER_SNAPSHOT_COUNT="$(snapshots_count)" -DEPSILON_APPS_NAMES=$(epsilon_app_names) -DUSERNAME="$(USERNAME)" -DAPPS_CAN_BE_HIDE_COUNT=$(apps_can_be_hide_count) -DAPPS_CAN_BE_HIDE_SETTINGS_MESSAGE_TREE_LIST="$(apps_can_be_hide_settings_message_tree_list)" -DAPPS_CAN_BE_HIDE_LIST="$(apps_can_be_hide_list_declaration)")
# $(info $(patsubst 'testetst tes', '\w+', '_(\&)'))

# $(error test)


$(call object_for,apps/apps_container_storage.cpp apps/apps_container.cpp apps/main.cpp): CXXFLAGS += $(snapshot_includes) -DAPPS_CONTAINER_APPS_DECLARATION="$(apps_declaration)" -DAPPS_CONTAINER_SNAPSHOT_DECLARATIONS="$(snapshots_declaration)" -DAPPS_CONTAINER_SNAPSHOT_CONSTRUCTORS="$(snapshots_construction)" -DAPPS_CONTAINER_SNAPSHOT_LIST="$(snapshots_list)" -DAPPS_CONTAINER_SNAPSHOT_COUNT="$(snapshots_count)" -DEPSILON_APPS_NAMES=$(epsilon_app_names) -DAPPS_CAN_BE_HIDE_COUNT=$(apps_can_be_hide_count) -DAPPS_CAN_BE_HIDE_SETTINGS_MESSAGE_TREE_LIST="$(apps_can_be_hide_settings_message_tree_list)" -DAPPS_CAN_BE_HIDE_LIST="$(apps_can_be_hide_list_declaration)" -DUSERNAME="$(USERNAME)"


# I18n file generation

# The header is refered to as <apps/i18n.h> so make sure it's findable this way
SFLAGS += -I$(BUILD_DIR)

i18n_files += $(addprefix apps/language_,$(addsuffix .universal.i18n, $(EPSILON_I18N)))
i18n_files += $(addprefix apps/,\
  shared.de.i18n\
  shared.en.i18n\
  shared.es.i18n\
  shared.fr.i18n\
  shared.pt.i18n\
  shared.hu.i18n\
  shared.universal.i18n\
  toolbox.de.i18n\
  toolbox.en.i18n\
  toolbox.es.i18n\
  toolbox.fr.i18n\
  toolbox.pt.i18n\
  toolbox.hu.i18n\
  variables.de.i18n\
  variables.en.i18n\
  variables.es.i18n\
  variables.fr.i18n\
  variables.pt.i18n\
  variables.hu.i18n\
)

$(eval $(call rule_for, \
  I18N, \
  apps/i18n.cpp, \
  $(i18n_files), \
  $$(PYTHON) apps/i18n.py --codepoints $(code_points) --header $$(subst .cpp,.h,$$@) --implementation $$@ --locales $$(EPSILON_I18N) --files $$^, \
  global \
))



$(BUILD_DIR)/apps/i18n.h: $(BUILD_DIR)/apps/i18n.cpp

# Handle PNG files

$(eval $(call depends_on_image,apps/title_bar_view.cpp,apps/exam_icon.png))

$(call object_for,$(apps_src) $(tests_src)): $(BUILD_DIR)/apps/i18n.h
$(call object_for,$(apps_src) $(tests_src)): $(BUILD_DIR)/python/port/genhdr/qstrdefs.generated.h

apps_tests_src = $(app_calculation_test_src) $(app_code_test_src) $(app_probability_test_src) $(app_regression_test_src) $(app_sequence_test_src) $(app_shared_test_src) $(app_statistics_test_src) $(app_settings_test_src) $(app_solver_test_src)

apps_tests_src += $(addprefix apps/,\
  global_preferences.cpp \
)

$(foreach img,$(image_list), $(eval $(call rule_for, \
  ICON, \
  $(img), \
  $(addprefix themes/themes/, $(addsuffix .json, $(OMEGA_THEME))), \
  $$(PYTHON) themes/themes_manager.py -i $(OMEGA_THEME) $$@ $(BUILD_DIR)/, \
  global \
)))
