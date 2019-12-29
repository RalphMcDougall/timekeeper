# The Analyzer tracks some given folders and then automatically generates timing graphs for new CSV files in those folders

import time, os, math
import matplotlib.pyplot as plt

FOLDERS = []
LAST_FOLDER_UPDATE = {}

def start():
    global FOLDERS, LAST_FOLDER_UPDATE
    loadWatchFolders()
    options = [
            "Display folder paths", 
            "Add folder path", 
            "Remove folder path",
            "Clear all folders",
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
            LAST_FOLDER_UPDATE[path] = 0
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
            # Start looking for updates to the folders
            watchFolders()
            break
        
        print("")




def loadWatchFolders():
    # Check the tracking text file to find which folders to follow
    global FOLDERS, LAST_FOLDER_UPDATE
    f = open("tracking.txt", "r")
    for line in f.readlines():
        name, lastUpdate = line.split(",")
        FOLDERS.append(name)
        LAST_FOLDER_UPDATE[name] = float(lastUpdate)
    f.close()


def saveWatchFolders():
    global FOLDERS, LAST_FOLDER_UPDATE
    f = open("tracking.txt", "w")
    for fp in FOLDERS:
        l = fp + "," + str(LAST_FOLDER_UPDATE[fp])
        f.write(l + "\n")
    f.close()


def watchFolders():
    # Look for changes in each folder
    global FOLDERS, LAST_FOLDER_UPDATE
    print("Watching Folders:", FOLDERS)

    while True:
        updated = False
        for fp in FOLDERS:
            files = []
            fp_files = fp + "/timekeeper_files"
            for f in os.listdir(fp_files):
                file_path = os.path.join(fp_files, f)
                if os.path.isfile(file_path) and file_path.endswith(".csv") and os.path.getmtime(file_path) > LAST_FOLDER_UPDATE[fp]:
                    files.append(file_path)

            if len(files) != 0:
                print("Found", len(files), "new", "files" if len(files) > 1 else "file", "in", fp)

                for path in files:
                    process(path)

                LAST_FOLDER_UPDATE[fp] = time.time()
                updated = True

        if updated:
            saveWatchFolders()
        
        time.sleep(2) # Only check for updates periodically


def process(file_path):
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

    assert lines[0].split(",")[0] == "project_name", "Invalid file format: no project name"
    assert lines[1].split(",")[0] == "program_name", "Invalid file format: no program name"

    project_name = lines[0].split(",")[1]
    program_name = lines[1].split(",")[1]

    print("Project name:", project_name)
    print("Program name:", program_name)

    # Process the events
    for l in lines[3:]:
        if time.time() - last_update > 5:
            # Display a progress message
            print("Processed: " + str(progress) + "/" + str(len(lines)) + " \t(" + str(round(100 * progress / len(lines), 2)) + "%)")
            last_update = time.time()
        ts, et, iss = l.split(",")
        ts = int(ts)
        et = int(et)
        event_times.append(ts)
        event_counts.append(len(event_counts))
        last_event_time = ts
        if iss not in issuers:
            issuers.append(iss)
            issue_times[iss] = 0
            intervals[iss] = []
        
        if et == 0: # Start event
            event_stack.append( [ts, et, iss] )
        
        elif et == 1: # End event
            le = event_stack.pop()

            if le[2] != iss:
                print("ERROR: Stack event mismatch. Stopping analysis.")
                return
            issue_times[iss] += ts - le[0]
            intervals[iss].append( [le[0], ts - le[0]] )
        
        progress += 1
    
    if len(event_stack) != 0:
        print("WARNING: Event stack still contains " + str(len(event_stack)) + " unprocessed " + ("items" if len(event_stack) > 1 else "item") )
        print(event_stack)

    print("Making time units readable")
    max_time_val = last_event_time
    unit_step = 0
    units = ["ns", "us", "ms", "s"]

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
    time_unit = units[unit_step]


    # Create visualisation
    print("Creating data visualisation")
    fig, axs = plt.subplots(2, 2)
    fig.suptitle( (file_path.split("/"))[-1].split(".")[0] )
    all_colours = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']
    used_colours = []
    for i in range(len(issuers)):
        used_colours.append( all_colours[i % len(all_colours)] )
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
    plt.show()
    plt1.savefig(".".join(file_path.split(".")[:-1]) + ".png")

    print("Successfully processed")



start()