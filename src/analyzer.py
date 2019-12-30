# The Analyzer tracks some given folders and then automatically generates timing graphs for new CSV files in those folders

import time, os, math
import matplotlib.pyplot as plt

FOLDERS = []
PROJECTS = {}

# Start enum
TRACKING_START = 0
TRACKING_END = 1
PROJECT_TRACKING_START = 2
PROJECT_TRACKING_END = 3
# End enum

PROJECTS_AWAITING_PROCESSING = []
COLOURS = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']
UNITS = ["ns", "us", "ms", "s"]

SILENT = False  # If it is in silent mode, it won't display the graphs automatically
PROJECT_WATCH_ONLY = False  # Only process data for the projects. Individual runs don't need to be processed

class Project:
    def __init__(self, project_name, bfp):
        print("Initialising new project")
        self.name = project_name
        self.base_folder_path = bfp

        self.issuers = {}
        self.programs = []
    
    def add_entry(self, issuer, program_name, flag, t):
        if issuer not in self.issuers.keys():
            self.issuers[issuer] = {}
        if program_name not in self.programs:
            self.programs.append(program_name)
        
        if program_name not in self.issuers[issuer].keys():
            self.issuers[issuer][program_name] = []

        self.issuers[issuer][program_name].append( [flag, t] )
    
    def save(self):
        print("Saving project:", self.name)
        f = open(self.base_folder_path + "/timekeeper/" + self.name + ".proj", "w")
        for iss in self.issuers.keys():
            rec = self.issuers[iss]
            for prog_name in rec.keys():
                dat = rec[prog_name]
                for i in dat:
                    f.write(iss + "," + prog_name + "," + str(i[0]) + "," + str(i[1]) + "\n")
        f.close()
        print("Successfully saved")


def start():
    global FOLDERS, SILENT, PROJECT_WATCH_ONLY
    loadWatchFolders()
    options = [
            "Display folder paths", 
            "Add folder path", 
            "Remove folder path",
            "Clear all folders",
            "Activate silent mode",
            "Activate project-only mode",
            "Run analysis"
    ]

    while True:
        for i in range(len(options)):
            print(i + 1, " - ", options[i])
        opt = int(input("-> "))

        if opt == 1:
            if len(FOLDERS) == 0:
                print("No folders being tracked")
            
            for i in range( len(FOLDERS) ):
                print(i + 1, " : ", FOLDERS[i])
        elif opt == 2:
            path = input("Path: ")
            FOLDERS.append(path)
            saveWatchFolders()
            print("Added new folder")
        elif opt == 3:
            ind = int(input("Folder path index: "))
            FOLDERS.remove(FOLDERS[ind - 1])
            saveWatchFolders()
            print("Removed folder path")
        elif opt == 4:
            FOLDERS = []
            saveWatchFolders()
            print("Cleared folders")
        elif opt == 5:
            SILENT = True
            print("Silent mode activated")
        elif opt == 6:
            PROJECT_WATCH_ONLY = True
            print("Activated project-only mode")
        elif opt == 7:
            # Start looking for updates to the folders
            watchFolders()
            break
        
        print("")


def loadWatchFolders():
    # Check the tracking text file to find which folders to follow
    global FOLDERS
    f = open("tracking.txt", "r")
    for line in f.readlines():
        name = line
        if "\n" in name:
            name = name[:-1]
        FOLDERS.append(name)
    f.close()


def saveWatchFolders():
    global FOLDERS
    f = open("tracking.txt", "w")
    for fp in FOLDERS:
        l = fp
        f.write(l + "\n")
    f.close()


def load_project(file_path):
    project_name = file_path.split("/")[-1][:-5]
    print("Loading project:", project_name)

    PROJECTS[project_name] = Project(project_name, "".join(file_path.split("/timekeeper/" + project_name + ".proj")[:-1]))

    f = open(file_path, "r")
    for line in f.readlines():
        line = line.split(",")
        iss = line[0]
        prog = line[1]
        flag = int(line[2])
        t = int(line[3])
        PROJECTS[project_name].add_entry(iss, prog, flag, t)
    f.close()
    print("Project loaded")




def watchFolders():
    # Look for changes in each folder
    global FOLDERS, PROJECTS_AWAITING_PROCESSING
    print("Watching Folders:", FOLDERS)

    while True:
        updated = False
        for fp in FOLDERS:
            files = []
            fp_files = fp + "/timekeeper"
            all_files = os.listdir(fp_files)
            for f in all_files:
                file_path = os.path.join(fp_files, f)
                if os.path.isfile(file_path) and file_path.endswith(".csv"):
                    files.append(file_path)
                
                if os.path.isfile(file_path) and file_path.endswith(".proj") and file_path.split("/")[-1][:-5] not in PROJECTS.keys():
                    load_project(file_path) 

            if len(files) != 0:
                print("Found", len(files), "new", "files" if len(files) > 1 else "file", "in", fp)

                for path in files:
                    process(path)

                updated = True

        if updated:
            saveWatchFolders()
        
        # Process projects
        for project_name in PROJECTS_AWAITING_PROCESSING:
            process_project(project_name)
        PROJECTS_AWAITING_PROCESSING = []
        
        time.sleep(2) # Only check for updates periodically


def process(file_path):
    global FOLDERS, PROJECTS_AWAITING_PROCESSING
    print("Processing:", file_path)
    csv_file = open(file_path, "r")
    lines = csv_file.readlines()
    csv_file.close()
    print("Data retrieved from CSV file")

    issuers = []
    issue_times = {}

    event_stack = []
    intervals = {}
    last_event_time = 0

    event_times = [0]
    event_counts = [0]

    progress = 0
    last_update = time.time()

    project_updated = False

    assert lines[0].split(",")[0] == "project_name", "Invalid file format: no project name"
    assert lines[1].split(",")[0] == "program_name", "Invalid file format: no program name"

    project_name = lines[0].split(",")[1]
    program_name = lines[1].split(",")[1]

    if "\n" in project_name:
        project_name = project_name[:-1]

    if "\n" in program_name:
        program_name = program_name[:-1]

    print("Project name:", project_name)
    print("Program name:", program_name)

    if project_name not in PROJECTS.keys():
        print("Creating new project")
        PROJECTS[project_name] = Project(project_name, "/".join(file_path.split("/")[:-2]))

    # Process the events
    for l in lines[3:]:
        if time.time() - last_update > 5:
            # Display a progress message
            print("Processed: " + str(progress) + "/" + str(len(lines)) + " \t(" + str(round(100 * progress / len(lines), 2)) + "%)")
            last_update = time.time()

        ts, et, iss, flag = l.split(",")
        ts = int(ts)
        et = int(et)
        flag = int(flag)
        event_times.append(ts)
        event_counts.append(len(event_counts))
        last_event_time = ts
        if iss not in issuers:
            issuers.append(iss)
            issue_times[iss] = 0
            intervals[iss] = []
        
        if et in (TRACKING_START, PROJECT_TRACKING_START): # Start event
            event_stack.append( [ts, et, iss, flag] )
        
        elif et in (TRACKING_END, PROJECT_TRACKING_END): # End event
            le = event_stack.pop()

            if le[2] != iss:
                print("ERROR: Stack event mismatch. Stopping analysis.")
                return
            
            time_diff = ts - le[0]
            issue_times[iss] += time_diff
            intervals[iss].append( [le[0], time_diff] )

            if et == PROJECT_TRACKING_END:
                add_to_project(project_name, program_name, iss, flag, time_diff)
                project_updated = True
        
        progress += 1
    
    if len(event_stack) != 0:
        print("WARNING: Event stack still contains " + str(len(event_stack)) + " unprocessed " + ("items" if len(event_stack) > 1 else "item") )
        print(event_stack)

    if project_updated:
        print("Project will be processed in the next update cycle")
        if project_name not in PROJECTS_AWAITING_PROCESSING:
            PROJECTS_AWAITING_PROCESSING.append(project_name)

    if PROJECT_WATCH_ONLY:
        # Don't create the visualisations for each program
        print("Skipping program visualisation")
        print("Removing CSV file")
        os.remove(file_path)
        return


    print("Making time units readable")
    max_time_val = last_event_time
    unit_step = 0

    while max_time_val >= 1000:
        # Change all other times to fit with the new unit
        event_times = [round(i / 1000, 3) for i in event_times]
        for iss in issuers:
            issue_times[iss] = round(issue_times[iss] / 1000, 3)
            for i in range(len(intervals[iss])):
                intervals[iss][i][0] = round(intervals[iss][i][0] / 1000, 3)
                intervals[iss][i][1] = round(intervals[iss][i][1] / 1000, 3)
        
        last_event_time = round(last_event_time / 1000, 3)

        unit_step += 1
        max_time_val /= 1000
    time_unit = UNITS[unit_step]


    # Create visualisation
    print("Creating data visualisation")
    fig, axs = plt.subplots(2, 2)
    fig.suptitle( program_name )
    used_colours = []
    for i in range(len(issuers)):
        used_colours.append( COLOURS[i % len(COLOURS)] )
    iss_times = [issue_times[i] for i in issuers]
    print("Setup completed")

    axs[0,0].axis("off")
    axs[0,0].table(
        cellText=[[issue_times[i]] for i in issuers], 
        rowLabels=issuers,
        rowLoc="center",
        rowColours=used_colours, 
        colLabels=["Execution time (" + time_unit +")"], 
        loc="center",
        cellLoc="center",
    ).scale(1, 2)
    axs[0,0].set_title("Issuer time table")
    print("Table completed")


    axs[0,1].bar(issuers, iss_times, color=used_colours)
    axs[0,1].set_title("Issuer time chart")
    axs[0,1].set_ylabel("Time (" + time_unit +")")
    pt = 10 ** int(math.log10(max(iss_times)))
    axs[0,1].set_ylim(0, pt * math.ceil(max(iss_times) / pt))
    print("Chart completed")

    cnt = 0
    for i in issuers:
        axs[1,0].broken_barh(intervals[i], (cnt, 1), color=used_colours[cnt])
        cnt += 1
    axs[1,0].set_yticks([(i + 0.5) for i in range(len(issuers))])
    axs[1,0].set_yticklabels(issuers)
    axs[1,0].set_xlabel("Time (" + time_unit +")")
    axs[1,0].set_title("Execution timeline")
    axs[1,0].set_xlim(0, last_event_time)
    print("Timeline completed")

    axs[1,1].plot(event_times, event_counts)
    axs[1,1].set_title("Cumulative event count")
    axs[1,1].set_xlabel("Time (" + time_unit +")")
    axs[1,1].set_ylabel("Event count")
    axs[1,1].set_xlim(0, last_event_time)
    axs[1,1].set_ylim(0, len(event_counts))
    print("Event count completed")

    plt.subplots_adjust(left=0.2, wspace=0.5, hspace=0.5)
    plt1 = plt.gcf()
    if not SILENT: plt.show()
    plt1.savefig(".".join(file_path.split(".")[:-1]) + ".png")

    print("Removing CSV file")
    os.remove(file_path)

    print("Successfully processed")


def add_to_project(project_name, program_name, issuer, flag, t):
    PROJECTS[project_name].add_entry(issuer, program_name, flag, t)


def process_project(project_name):
    print("Processing project:", project_name)
    proj = PROJECTS[project_name]
    plt.clf()
    for i in range(len(proj.issuers.keys())):
        plt.subplot(1, len(proj.issuers.keys()), i + 1)
        max_x = 0
        max_y = 0
        k = list(proj.issuers.keys())[i]
        
        # Fix the time units
        time_unit_ind = 0
        max_t = 0
        scale = 1
        for prog_name in proj.issuers[k].keys():
            y = [j[1] for j in proj.issuers[k][prog_name]]
            max_t = max(max_t, max(y))
        
        while max_t >= 1000:
            scale *= 1000
            time_unit_ind += 1
            max_t = round(max_t / 1000, 3)

        cnt = 0
        for prog_name in proj.issuers[k].keys():
            x = [j[0] for j in proj.issuers[k][prog_name]]
            y = [round(j[1] / scale, 3) for j in proj.issuers[k][prog_name]]
            max_x, max_y = max(max_x, max(x)), max(max_y, max(y))

            plt.scatter(x, y, c=COLOURS[cnt % len(COLOURS)], label=prog_name)
            cnt += 1

        plt.title(k)
        ptx = 10 ** (math.floor( math.log10(max_x) ))
        plt.xlim(0, ptx * math.ceil(max_x / ptx))
        plt.xlabel("Flag value")
        pty = 10 ** (math.floor( math.log10(max_y) ))
        plt.ylim(0, pty * math.ceil(max_y / pty))
        plt.ylabel("Execution time (" + UNITS[time_unit_ind] + ")")
        plt.legend()


    plt1 = plt.gcf()
    if not SILENT: plt.show()
    plt1.savefig(proj.base_folder_path + "/timekeeper/" + proj.name + ".png")
    PROJECTS[project_name].save()
    print("Project processed")


start()