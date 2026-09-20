# CO2203 Group Project — Final Project Report

**Group: "The Unhinged"**

Okith Lunuwila – 24/ENG/105
Thishal Wickramathilake – 24/ENG/162
Nithika Dodangoda – 24/ENG/034

University Course Registration, Timetable and Attendance Management system (CRTAM).
Written in C++17, built with GNU Make, no external libraries.

---

## 1. Final UML class diagram

The final diagram is `docs/uml.puml` (PlantUML source, render at plantuml.com/plantuml).
It has been updated to match the code as submitted. The differences from the D1 design
diagram are listed in Section 2 and every one of them is recorded in `docs/CHANGELOG.md`.

The three structural changes worth naming here:

- **`UniversitySystem` was added.** It is absent from the D1 diagram. It is the singleton
  that owns the three repositories plus the attendance register, and it holds every
  enrolment rule. A diagram without it hides where the coordination happens.
- **`Repository<T>` stores `T*`, not `T`.** Storing objects by value would slice every
  subclass down to its base, which would break the whole polymorphic design.
- **`Menu ..> Repository` was replaced by `Menu ..> UniversitySystem`.** The menu never
  touches a repository directly.

---

## 2. Design change log

The full log is `docs/CHANGELOG.md`: 45 dated rows, each with the change, the reason and
the member who made it. Summarised by theme:

### 2.1 Changes forced by the persistence format

| Change | Reason |
|---|---|
| Every `toLine()` switched from `\|` to `,` separators and now writes all fields | The pipe format could not be read back by the data files, so a save/load cycle lost data |
| `Person::fromLine`, `Course::fromLine`, `Enrolment::fromLine` implemented | `Repository<T>::load` needs a `T*` factory per type |
| `Enrolment::fromLine` returns `Enrolment*` rather than `Enrolment` | The repository factory signature requires a pointer |
| `Enrolment` IDs generated as `ENR<n>`, counter advances past IDs already on disk | The data file already used `ENR1`/`ENR2`; without the bump a new enrolment would reuse an ID |
| Student card UID derived as `"CARD_" + id` | `users.txt` has no UID column, and the tap file uses `CARD_S001` style tags |
| `getCreditVal()` body moved out of `Course.h` | A function defined in a header is compiled into every `.cpp` that includes it, causing duplicate-symbol link errors |

### 2.2 Changes to the class interfaces

| Change | Reason |
|---|---|
| `Course` gained `slots`, `getSlots()`, `addSlot()`; `getPrereq` renamed `getPrerequisites` | Required by the clash check and the enrolment rules |
| `Student` gained `card`, `timetable`, `getCard()`, `getTimetable()`, `getCompletedCourses()`, `addCompletedCourse()` | Required for UID resolution, clash checking and prerequisite checking |
| `Timetable::addSlot(slot, courseId)` and `removeSlotsOf(courseId)` added, backed by a parallel `slotCourses` vector | Dropping a course has to remove exactly that course's slots |
| `ClashException` holds two `TimeSlot` objects instead of two strings | Matches the D1 diagram, and the message prints readable times through `TimeSlot::operator<<` |
| `markPresent` / `appendCorrection` gained a `notify` flag | A reload must stay silent; a live tap should print |
| `AttendanceRegister::processEvent` reduced to one version; the `std::function` overload removed | It now asks `UniversitySystem` for the UID lookup and the enrolment check itself, matching the single `processEvent` in the D1 diagram |
| `courseAttendanceReport` merged into one function | The D1 diagram declares one report; it now fetches the roll from `Course` instead of the caller supplying it |
| `Person::getPassword()` added, then made **protected** | `toLine()` must write the password or a save/load cycle wipes every account's credentials. Protected keeps the D1 claim "the password has no getter" true for every caller outside the hierarchy |

### 2.3 Divergences from the D1 documents

Each of these is a place where the submitted code does not match the D1 rationale or the
interface agreement. All are recorded in the change log with reasons.

| D1 said | The code does | Why |
|---|---|---|
| Repository instantiated for Student, Lecturer, Course, Enrolment, AttendanceSession | `Repository<Person>`, `<Course>`, `<Enrolment>` | One `Person` store is what lets `fromLine` return any role and `showDashboard` dispatch polymorphically. `AttendanceRegister` keeps its own sessions because `attendance.txt` is nested, which a flat one-object-per-line template cannot express |
| `fstream` is used inside `Repository` only | Also in `AttendanceRegister` and `FileReplayCapture` | The nested attendance format, and reading the tap file is `FileReplayCapture`'s entire purpose |
| The Menu is the only catch site | `UniversitySystem::initialize` also catches | A missing data file on a first run is not an error; catching `FileNotFoundException` there creates empty starter files |
| `AttendanceRegister` depends on `AttendanceCapture` | The **Menu** holds the `AttendanceCapture*` | The implementations are still interchangeable behind the base pointer; the console-or-file choice belongs where the user makes it |
| Prerequisite checking assigned to M2 | Implemented in `UniversitySystem::enrolStudent` with the other five rules | All six checks run as one ordered sequence over several repositories |
| `Repository<T>::load(path)` | `load(path, fromLineFactory)` | The template cannot parse a line without being handed the factory |

---

## 3. Concept mapping — Section 4 requirements

| Requirement | Where it is implemented |
|---|---|
| **Encapsulation** | All data members private: `Person.h:8-11` (id, name, password), `Course.h:12-20`, `Enrolment.h:10-15`, `AttendanceSession.h:14-25`. Access only through public getters |
| **Password never exposed** | `Person::login()` compares inside the class (`Person.cpp:31`). `getPassword()` is **protected** (`Person.h:14-17`) so only the subclasses' `toLine()` can read it |
| **Inheritance** | `Person <- Student, Lecturer, Administrator` (`Person.h:7`, `Student.h:11`, `Lecturer.h:11`, `Administrator.h:8`). `Course <- LectureCourse, LabCourse, ProjectCourse`. `AttendanceRecord <- CorrectionRecord`. `AttendanceCapture <- FileReplayCapture, ConsoleCardReader` |
| **Abstract classes** | `Person.h:28-29` (`showDashboard`, `toLine` pure virtual), `Course.h:48` (`calculateCredits`), `AttendanceCapture.h:10-14` (three pure virtuals) |
| **Polymorphism (FR1.3)** | `main.cpp:284` — `user->showDashboard()` on a `Person*`, no `if/else` on a role string. Credit rules: `LectureCourse.cpp:12`, `LabCourse.cpp:16`, `ProjectCourse.cpp:19` |
| **Virtual copy (clone)** | `AttendanceRecord.cpp:23` and `CorrectionRecord.cpp:17`; used by the session's deep copy at `AttendanceSession.cpp:18-22` |
| **Abstract interface / dependency inversion (FR7.6)** | `AttendanceCapture.h`. `main.cpp:134-160` holds one `AttendanceCapture*` and calls `captureNext()` through it, so a typed UID and a replayed file reach `processEvent` by the same path |
| **Composition** | `Course` owns `vector<TimeSlot> slots` (`Course.h:20`); `Student` owns `StudentCard` and `Timetable` (`Student.h:13-15`); `AttendanceRegister` owns `vector<AttendanceSession>` (`AttendanceRegister.h:16`); `AttendanceSession` owns `vector<AttendanceRecord*>` |
| **Aggregation** | `Course` holds `vector<string> enrolledIds` (`Course.h:18`) — students referenced by ID and existing independently of the course |
| **Operator overloading** | `TimeSlot::operator&&` — clash detection, `TimeSlot.cpp:25` (FR4.4). `Enrolment::operator<` — sorts by ID, `Enrolment.cpp:40`. `operator<<` on `Course` (`Course.cpp:161`), `TimeSlot` (`TimeSlot.cpp:61`), `Timetable` (`Timetable.cpp:46`), `Enrolment` (`Enrolment.cpp:44`), `AttendanceRecord` (`AttendanceRecord.cpp:58`) (FR6.2) |
| **Rule of Five** | `StudentCard` (raw `char* uid`): default ctor `StudentCard.cpp:4`, copy ctor `:15`, copy assignment `:26`, move ctor `:41`, move assignment `:47`, destructor `:57`. `AttendanceSession` (owns `AttendanceRecord*`): `AttendanceSession.cpp:15, 25, 49, 55, 74` |
| **Templates** | `Repository<T>` — `Repository.h:13`. Instantiated as `Repository<Person>`, `Repository<Course>`, `Repository<Enrolment>` (`UniversitySystem.h:20-22`) |
| **Static members (FR7.1)** | `Enrolment::nextId` (`Enrolment.cpp:6`) — one shared ID counter. `AttendanceSession::defaultDurationMin` (`AttendanceSession.cpp:8`). `UniversitySystem::instance` (`UniversitySystem.cpp:16`) — the singleton pointer. Static factory functions: `Person::fromLine`, `Course::fromLine`, `Enrolment::fromLine`, `AttendanceRecord::fromLine`, `AttendanceSession::fromLine`, `TimeSlot::fromLine` |
| **Exception hierarchy** | `Exceptions.h` — `SystemException` with four branches (`UserException`, `EnrolmentException`, `AttendanceException`, `PersistenceException`) and 11 leaf types |
| **Exceptions thrown** | `UniversitySystem.cpp:104, 107, 112, 114, 126, 135` (the six enrolment rules); `AttendanceRegister.cpp` (`SessionClosedException`, `NotEnrolledException`, `DuplicateAttendanceException`); `Repository.h:76, 93, 108` (persistence) |
| **Exceptions caught** | `main.cpp` — `catch (const SystemException& e)` in `run()` and in all three role menus, plus `UniversitySystem.cpp:30-35` for the first-run case |
| **File handling (FR5.2)** | `Repository.h:69` (`ofstream`) and `:86` (`ifstream`); `AttendanceRegister.cpp:190, 216` for the nested attendance format; `FileReplayCapture.cpp:14` for the tap file |
| **Clash detection (FR4.2, FR4.4)** | `Timetable::hasClash` (`Timetable.cpp:8`) calls `TimeSlot::operator&&`, which calls `overlaps()` — same day and the times cross |
| **Prerequisite checking** | `UniversitySystem.cpp:116-127` — a prerequisite counts as met if the student has completed it **or** is currently enrolled in it |
| **Capacity check** | `Course::isFull()` (`Course.cpp:46`), called at `UniversitySystem.cpp:114` |
| **Attendance capture (FR7.2)** | `AttendanceRegister::processEvent` (`AttendanceRegister.cpp:58`) — resolves UID, refuses a non-enrolled student, refuses a repeat tap, then records |
| **Attendance reporting (FR7.4)** | `attendancePercent` (`AttendanceRegister.cpp:104`) and `courseAttendanceReport` (`:129`), including the 80% eligibility threshold |
| **Const correctness** | Every getter and query function is `const`; `save()` is `const`, `load()` is not. Shown as `{query}` in the diagram |
| **Singleton** | `UniversitySystem` — private constructor (`UniversitySystem.h:26`), static `instance` (`UniversitySystem.cpp:16`), `getInstance()` (`:20`) |

---

## 4. File format used for persistence

Four plain-text files in `data/`. One object per line, comma separated, with the first
field acting as a type tag where a hierarchy is being stored. Nested fields inside a
column use `;` and `|` so they never collide with the top-level comma.

### 4.1 `users.txt` — `Role,id,name,password[,completedCourses]`

```
Admin,A001,Okith Lunuwila,admin123
Lecturer,L001,Dr. Nithika Dodangoda,lec123
Student,S001,Thishal Wickramathilake,stu123,CS100
Student,S002,Kamal Perera,stu123,NONE
```

`Role` is `Student`, `Lecturer` or `Admin`; `Person::fromLine` reads it and constructs the
matching subclass. The 5th column exists only for students: completed courses separated by
`;`, or `NONE`. The card UID is not stored — it is derived as `"CARD_" + id`.

### 4.2 `courses.txt` — `TAG,code,title,credits,capacity,lecturerId,extra,prereq,slots`

```
LEC,CS101,Intro to Programming,3,30,L001,0,NONE,1;540;660;Hall 1
LAB,CS102,Data Structures Lab,2,25,L001,4,CS101,3;780;960;Lab B
PROJ,CS103,Software Capstone,4,20,L002,0,CS102,5;540;720;Room 3
```

- `TAG` is `LEC`, `LAB` or `PROJ` and selects the subclass.
- `extra` is subclass-specific: lab hours for `LAB`, `1`/`0` pass-fail for `PROJ`, `0` for `LEC`.
- `prereq` is `;` separated, or `NONE`.
- `slots` are `|` separated; each slot is `day;startMin;endMin;location` where day 0 is
  Sunday and the times are minutes from midnight (540 = 09:00). Time slots are stored
  **inline** rather than in their own file, because a slot belongs entirely to its course
  (FR5.2).

### 4.3 `enrolments.txt` — `id,studentId,courseId,date`

```
ENR1,S001,CS101,2026-09-01
ENR2,S002,CS101,2026-09-01
```

On load, `Enrolment::fromLine` advances the static `nextId` past every ID already on disk,
so a new enrolment can never reuse one.

### 4.4 `attendance.txt` — nested: a `SESSION` line then its record lines

```
SESSION,CS101_SESS_1,CS101,1;540;660;Hall 1,2026-09-17 10:00,10,1
RECORD,S001,CS101_SESS_1,2026-09-17 10:00,Present,CardTap
RECORD,S002,CS101_SESS_1,2026-09-17 10:00,Present,CardTap
CORRECTION,S003,CS101_SESS_1,2026-09-17 10:05,Present,Manual,L001,arrived late
```

- `SESSION,id,courseId,slot,openedAt,durationMin,isOpen`
- `RECORD,studentId,sessionId,timestamp,status,capturedBy`
- `CORRECTION,` the same six fields, then `actingLecturerId,reason`

A correction is appended **beside** the original record rather than replacing it, so the
history survives. This is the one file the generic `Repository<T>` cannot handle, because
it is two levels deep; `AttendanceRegister` reads and writes it itself.

### 4.5 `replay_taps.txt` — `uid,timestamp`

Input only, never written. Blank lines and lines starting with `#` are ignored.

### 4.6 What is *not* stored

Student timetables and course rolls are **derived, never saved**.
`UniversitySystem::rebuildStudentTimetables()` walks the enrolments at startup and rebuilds
both. `enrolments.txt` is the single source of truth, so the two can never drift apart.

---

## 5. Testing summary

All tests were run against `app.exe` built with `g++ -std=c++17 -Wall -Iinclude` (zero
warnings) on a copy of `data/`, so the repository files were not disturbed.

### 5.1 Save / load round trip

Start the program, exit immediately. All three files were reproduced **byte for byte**
(`diff` reported no differences), proving `toLine()` and `fromLine()` are exact inverses
for every class and every subclass tag.

### 5.2 Exception paths (six tested, five shown)

| # | Scenario | Exception | Output |
|---|---|---|---|
| 1 | `S001` enrols in `CS101`, already enrolled | `AlreadyEnrolledException` | `Error: Student is already enrolled in CS101.` |
| 2 | `S002` enrols in `CS103`, has not done `CS102` | `PrerequisiteNotMetException` | `Error: Cannot enrol in CS103. Missing prerequisite: CS102.` |
| 3 | Login as `S999` | `UserNotFoundException` | `Error: User with ID S999 was not found.` |
| 4 | Second tap from the same card in one session | `DuplicateAttendanceException` | `Error: Student has already marked attendance for this session.` |
| 5 | Tap from a student not on the course | `NotEnrolledException` | `Error: Cannot mark attendance because student is not enrolled in this course.` |
| 6 | Tap after the session is closed | `SessionClosedException` | `Error: Attendance session has already been closed or expired.` |

Admin validation was tested separately: duplicate user ID, unknown role and editing a
course code that does not exist were all refused with a message and no change written.

### 5.3 Clash detection (FR4.2 / FR4.4)

`S003` enrols in `CS104` (Mon 10:00–12:00, Hall 2) — accepted. `S003` then attempts
`CS101` (Mon 09:00–11:00, Hall 1):

```
 SYSTEM ALERT:
 Error: Timetable clash between Mon 09:00 - 11:00 (Hall 1) and Mon 10:00 - 12:00 (Hall 2).
```

The overlap is partial (10:00–11:00) and on the same day, which is exactly the case
`TimeSlot::overlaps` is written for. The enrolment was rejected **before** anything was
written, and the student's timetable still showed only the `CS104` slot afterwards.

### 5.4 `FileReplayCapture` run with valid and malformed events

Input file, deliberately containing five malformed lines:

```
CARD_S001,2026-09-17 09:05          <- valid
                                     <- blank line
CARD_S002                            <- no timestamp
# a comment line that must be skipped
CARD_S001,2026-09-17 09:10          <- duplicate tap
CARD_S003,2026-09-17 09:15          <- valid card, student not enrolled
GHOST_CARD_999,2026-09-17 09:20     <- unknown card
,2026-09-17 09:25                   <- empty UID
```

Result:

```
[FileReplay] ... UID: CARD_S001        Recorded: Student S001 marked present.
[FileReplay] ... UID: CARD_S002        Recorded: Student S002 marked present.
[FileReplay] ... UID: CARD_S001        Error: Student has already marked attendance...
[FileReplay] ... UID: CARD_S003        Error: Cannot mark attendance because student is not enrolled...
[FileReplay] ... UID: GHOST_CARD_999   Error: Cannot mark attendance because student is not enrolled...
[FileReplay] ... UID:                  Error: Cannot mark attendance because student is not enrolled...
```

Every malformed line was handled without a crash: blanks and comments skipped silently, a
missing timestamp defaulted, and an unknown or empty UID rejected through the normal
exception path. The two valid taps were recorded, written to `attendance.txt`, and read
back correctly on the next run. **The `try` block sits inside the capture loop**, so one
bad tap does not end the session.

### 5.5 End-to-end role tests

| Role | Exercised |
|---|---|
| Student | login, enrol, drop, view timetable, view attendance percentages |
| Lecturer | view own courses, view enrolment roll, open session, run card capture, close session, attendance report |
| Administrator | add/edit/remove user, add/edit/remove course, enrolment report |

FR2.3 was verified: `L002` attempting to view `CS101`, which is assigned to `L001`, was
refused with `Course CS101 is not assigned to you.`

Editing was checked for data preservation — editing `CS105` changed title, capacity and
lecturer while keeping its prerequisite and time slot; editing `S003` changed name and
password while keeping the card UID and completed courses, and the new password worked on
the next login.

### 5.6 Additional scenarios

Beyond the required cases, the following were also run:

| Scenario | Result |
|---|---|
| First run with no `data/` folder at all | Folder and four empty files created through the `FileNotFoundException` path; no crash |
| Course capacity | A capacity-1 course was created, a second student refused with `Course TINY1 is full. Maximum capacity is 1.` |
| Credit rules across all three course types | `LectureCourse` 3, `LabCourse` 2 + 4/3 = 3, `ProjectCourse` 4, and a pass/fail project 0 — the same `calculateCredits()` call through a `Course*` giving four different answers |
| Session lifecycle | Open → tap → close → reopen → tap; session IDs incremented `_SESS_1`, `_SESS_2`, and the closed session refused further taps |
| Prerequisite satisfied by current enrolment | `S003` enrolled in `CS102` and was then allowed into `CS103`, which requires it — a prerequisite counts as met if it is completed **or** in progress |
| Drop a course the student is not in | Refused, nothing written |
| Admin validation | Duplicate user ID, unknown role and editing a non-existent course code all refused with a message and no change written |

### 5.7 Defects found by testing, and the fixes

Testing was not only confirmation. Three defects were found and fixed, which is the main
argument for having run the sweep at all.

**1. Data loss on a corrupt data file (serious).**
A single unparseable line in `courses.txt` made `loadAll()` throw part way through, so
`enrolments.txt` and `attendance.txt` were never loaded. The save on exit then wrote both
files out **empty**, destroying every enrolment and attendance record. Reproduced by
appending one junk line to `courses.txt`.

*Fix:* `UniversitySystem` records the failure and `saveAll()` refuses to write:

```
[System Alert] Error: Corrupt data found in file data/courses.txt at line 6.
[System Alert] Saving is disabled until that line is fixed.
[System Alert] Not saving: the data files were not fully loaded.
```

After the fix, the same test left all three enrolment records untouched. The principle:
a program that cannot read its data must not be allowed to write it.

**2. Corrupt-line errors always reported line 0.**
`Person::fromLine`, `Course::fromLine` and `Enrolment::fromLine` each threw
`CorruptDataException(file, 0)`, so the message never told you which line was broken.

*Fix:* the factories now return `nullptr` on a bad line. `Repository<T>::load` already
counts lines, so it raises the exception itself with the true line number — which is how
the message above says "line 6". This also removed three duplicate throw sites.

**3. Non-numeric input silently produced a corrupt record.**
Typing letters at a numeric prompt (for example `abc` for credits) put `cin` into a fail
state. Every later read was skipped without complaint, and the course was still created
and saved:

```
LEC,BAD1,Bad Course,0,0,,0,,1;540;660;      <- 0 credits, 0 capacity, no lecturer, no room
```

*Fix:* the seven numeric prompts in `Administrator` now go through one file-level
`readNumber()` that clears the stream and throws `That field needs a whole number.`
The record is not created. The menu's own `readChoice()` already handled this correctly;
only the admin data-entry prompts were exposed.

All three fixes were verified by re-running the tests that exposed them, followed by the
full regression above.

---

## 6. Individual contribution table

*To be completed and signed by all three members before submission.*

| Member | Contribution | Approx. share | Signature |
|---|---|---|---|
| Okith Lunuwila (24/ENG/105) | | | |
| Thishal Wickramathilake (24/ENG/162) | | | |
| Nithika Dodangoda (24/ENG/034) | | | |

---

## 7. Known limitations

1. **Timestamps are hard-coded.** `markPresent` writes `"2026-09-17 10:00"` and enrolments
   are dated `"2026-09-17"` regardless of when they happen. `<ctime>` would fix this, but
   every stored date would then change between runs and the save/load test could not check
   for byte-identical output.

2. **`isExpired()` ignores the clock.** It returns `!isOpen`, so a session expires only
   when a lecturer closes it — `durationMin` is stored and never enforced. With real
   timestamps it would become `now > openedAt + durationMin`.

3. **Passwords are stored in plain text** in `users.txt`. Any hash would do better, but
   hashing is out of scope for the module.

4. **The data files cannot contain a comma.** Names and titles are rejected if they
   contain one, because a comma would split the field into two columns. A quoted-CSV
   reader would remove the restriction.

5. **Twelve checks throw the base `SystemException`** with a hand-written message, seven of
   them the same `"Course 'X' not found."`, because the hierarchy has `UserNotFoundException`
   but no `CourseNotFoundException`. They are caught correctly, but a caller cannot react
   to them specifically.

6. **Only one attendance session can be active at a time.** `processEvent` uses
   `getActiveSession()` with no argument, which returns the most recently opened open
   session, so two courses running simultaneously would send taps to the wrong one.

7. **The singleton is never deleted.** `getInstance()` calls `new` with no matching
   `delete`. The memory is reclaimed when the program exits, so nothing leaks while
   running, but it is not symmetric.

8. **Numeric fields are checked for type but not for range.** Non-numeric input is now
   rejected (Section 5.7), but a capacity of `-5` or a start time of `9999` minutes is
   still accepted when creating a course.

9. **Single user, single session.** There is no concurrency handling; the files are
   rewritten wholesale on every save.

---

## 8. Possible improvements

1. **Add `CourseNotFoundException`, `NotAssignedException` and `SessionNotFoundException`.**
   This is the smallest change with the biggest payoff — it removes all twelve base-class
   throws and would let `SystemException` become genuinely abstract, as the D1 diagram
   claims.

2. **Real timestamps via `<ctime>`,** which then makes `isExpired()` honour `durationMin`
   and lets attendance be reported by date.

3. **Sessions keyed by course.** `processEvent(courseId, event)` using
   `getActiveSession(courseId)` — the overload already exists and is currently unused —
   would allow several courses to take attendance at once.

4. **Smart pointers.** `vector<unique_ptr<AttendanceRecord>>` and
   `map<string, unique_ptr<T>>` would remove every manual `delete` and make the rule of
   five in `AttendanceSession` unnecessary. The raw pointers were kept deliberately, since
   demonstrating the rule of five is a module requirement.

5. **A quoted CSV reader,** removing the comma restriction on names and titles.

6. **Unit tests.** All testing was done through the menu by hand. A small test harness
   calling `UniversitySystem` directly would make regressions visible immediately — this
   is how the enrolment rules were checked during development.

7. **`Repository<AttendanceSession>`,** if the attendance file were flattened so each
   record carried its own session ID. That would put every file through one code path and
   make the D1 claim about `fstream` true.

8. **Separate the menu from the I/O.** `Menu` both asks questions and prints results; a
   thin view layer would make the logic testable without stdin.

9. **Password hashing,** even a simple one, to stop `users.txt` being a plain-text
   credential list.
