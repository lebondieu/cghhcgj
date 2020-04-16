import sys

resource_requested = sys.argv[1]

MACROS_FILE_INIT = "#ifndef MACROS_H\n#define MACROS_H\n"

MACROS_FILE_END = "#endif"

MACROS = "#define {} {}\n"

MACRO_FILE_PATH = "macros.h"

def filter_apps_list(raw_app_list, model, force_external):
	if force_external or model == "n0110":
		return " ".join(raw_app_list)
	else:
		result = ""
		for app in raw_app_list:
			if app != "external":
				result += app
				result += " "

		return result

def filter_apps_can_be_hide_count_list(raw_app_list, model, force_external, raw_app_can_be_hide_list):
		assert(len(raw_app_can_be_hide_list) == len(raw_app_list))

		if force_external or model == "n0110":
			return " ".join(raw_app_can_be_hide_list)
		else:
			result = ""
			for i, app in enumerate(raw_app_list):
				if app != "external":
					result += raw_app_can_be_hide_list[i]
					result += " "

			return result

def snapshots_declaration(app_list, apps_can_be_hide):
		return " ".join(
				["{0}::App::Snapshot m_snapshot{0}AppSnapshot;".format(app.title()) for app in app_list]
			)

def apps_declaration(app_list, apps_can_be_hide):
		return " ".join(
				["{0}::App m_{0}App;".format(app.title()) for app in app_list]
			)

def snapshots_construction(app_list, apps_can_be_hide):
		return "".join(
				[", m_snapshot{0}AppSnapshot()".format(app.title()) for app in app_list]
			)

def snapshots_list(app_list, apps_can_be_hide):
		return "".join(
				[", &m_snapshot{0}AppSnapshot".format(app.title()) for app in app_list]
			)

def snapshots_count(app_list, apps_can_be_hide):
		return len(app_list)

def snapshot_includes(app_list, apps_can_be_hide):
		return " ".join(
				["-include apps/{0}/app.h".format(app) for app in app_list]
			)

def epsilon_app_names(app_list, apps_can_be_hide):
		return "\'" + "".join(
				["\"{0}\", ".format(app) for app in app_list]
			) + "\'"

def apps_can_be_hide_count(app_list, apps_list_can_be_hide):
		count = 0
		for app in apps_list_can_be_hide:
			if app == "1":
				count += 1
		return count

def apps_can_be_hide_settings_message_tree_list(app_list, apps_list_can_be_hide):
		result_list = []
		for i, app in enumerate(apps_list_can_be_hide):
			if app == "1":
				result_list.append("SettingsMessageTree(I18n::Message::{}App)".format(app_list[i].title()))
		return ', '.join(result_list)

def apps_can_be_hide_list(app_list, apps_list_can_be_hide):
	return ", ".join(
				[("false", "true")[app == "1"] for app in apps_list_can_be_hide]
			)

MACROS_TO_GENERATE = {
	"APPS_CONTAINER_APPS_DECLARATION": apps_declaration,
	"APPS_CONTAINER_SNAPSHOT_DECLARATIONS": snapshots_declaration,
	"APPS_CONTAINER_SNAPSHOT_CONSTRUCTORS": snapshots_construction,
	"APPS_CONTAINER_SNAPSHOT_LIST": snapshots_list,
	"APPS_CONTAINER_SNAPSHOT_COUNT": snapshots_count,
	"EPSILON_APPS_NAMES": epsilon_app_names,
	"APPS_CAN_BE_HIDE_COUNT": apps_can_be_hide_count,
	"APPS_CAN_BE_HIDE_LIST": apps_can_be_hide_list,
	"APPS_CAN_BE_HIDE_SETTINGS_MESSAGE_TREE_LIST": apps_can_be_hide_settings_message_tree_list
}

if resource_requested in ("filter_apps_list", "filter_apps_can_be_hide_count_list"):

	raw_app_list = sys.argv[2].split()
	model = sys.argv[3]
	force_external = (False, True)[sys.argv[4] == "1"]

	if resource_requested == "filter_apps_list":
		print(filter_apps_list(raw_app_list, model, force_external))

	elif resource_requested == "filter_apps_can_be_hide_count_list":
		raw_app_can_be_hide_list = sys.argv[5].split()
		print(filter_apps_can_be_hide_count_list(raw_app_list, model, force_external, raw_app_can_be_hide_list))

elif resource_requested == "snapshot_includes":
	app_list = sys.argv[2].split()
	print(snapshot_includes(app_list, None))

elif resource_requested == "create_macros":
	app_list = sys.argv[2].split()
	apps_list_can_be_hide = sys.argv[3].split()

	with open(MACRO_FILE_PATH, "w") as fiw:
		fiw.write(MACROS_FILE_INIT)

		for macro in MACROS_TO_GENERATE:
			fiw.write(MACROS.format(macro, MACROS_TO_GENERATE[macro](app_list, apps_list_can_be_hide)))

		fiw.write(MACROS_FILE_END)

