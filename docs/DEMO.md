# Demo script — CO2203 viva (F1, 5 marks, individual)

The rubric wants a **confident, complete demo covering all features including
failure/exception paths, within time**. This script runs in about **10 minutes** and shows
every feature plus **nine** exception paths.

It is individual, so each member runs the whole thing. **Reset the data between members**
(last section).

---

## Setup (do this before the examiner is watching)

```powershell
cd C:\Users\ASUS\unhinged
git checkout data/          # clean starting data
mingw32-make                # should print one g++ line, no errors
```

Open a **second** PowerShell in the same folder for showing the files later.

Start it:

```powershell
.\app.exe
```

**Logins:** `S001`/`stu123`, `S002`/`stu123`, `L001`/`lec123`, `L002`/`alan123`, `A001`/`admin123`

---

## Act 1 — Student: enrolment rules (3 min)

Log in: `1` → `S001` → `stu123`

| Type | What appears | Say |
|---|---|---|
| `3` | CS101 and L432 slots | "Timetables are never stored. They're rebuilt from the enrolments every startup." |
| `1` then `CS102` | `[Success] Enrolled in CS102` | "CS102 needs CS101. He's enrolled in it now, and a prerequisite counts as met if it's completed **or** in progress." |
| `1` then `CS102` | **`AlreadyEnrolledException`** | "First failure path." |
| `1` then `CS104` | **`ClashException`** with both times printed | "CS104 is Monday 10–12, CS101 is Monday 9–11. The check is `if (existing && newSlot)` — `operator&&` on TimeSlot." |
| `3` | CS102 now listed, tagged `[CS102]` | |
| `2` then `CS102` | `[Success] Dropped course CS102` | "The tag is how `removeSlotsOf` knows which slots to pull." |
| `3` | CS102 gone | |
| `5` | back to main menu | |

Log in: `1` → `S002` → `stu123`

| Type | What appears | Say |
|---|---|---|
| `1` then `CS103` | **`PrerequisiteNotMetException`** — missing CS102 | "Third failure path. He hasn't done CS102 and isn't taking it." |
| `5` | | |

---

## Act 2 — Lecturer and attendance (4 min, the centrepiece)

Log in: `1` → `L001` → `lec123`

| Type | What appears | Say |
|---|---|---|
| `1` | CS101 **3**, CS102 **3**, L432 **3** | "CS102 is a LAB: 2 credits plus 4 lab hours ÷ 3 = 3. Same `calculateCredits()` call through a `Course*`, three subclasses, different answers." |
| `2` then `CS101` | S001 and S002 with names | |
| `3` then `CS101` | Session `CS101_SESS_1` opens | |
| then `file` | the replay runs | **stay quiet and let it play** |

The replay prints:

```
UID: CARD_S001   Recorded: Student S001 marked present.
UID: CARD_S002   Recorded: Student S002 marked present.
UID: CARD_S001   Error: Student has already marked attendance for this session.
UID: CARD_S003   Error: Cannot mark attendance because student is not enrolled...
```

Say: *"Four taps. `CARD_S001` is resolved to the student, the repeat tap is refused, and
S003 isn't enrolled in CS101 so he's refused too. Two more failure paths."*

| Type | What appears | Say |
|---|---|---|
| `5` then `CS101` | per-session list **and** per-student table, both 100% | "The roll comes from Course — the register only knows who tapped, not who should have." |
| `3` then `CS101` then `console` | live tap station | Type `CARD_S001`, then `S002`, then `stop` |
| | both accepted | "Same code path. One `AttendanceCapture*` holds either reader — typed or replayed, it makes no difference downstream." |
| `4` then `CS101_SESS_2` | `Session ... is now closed.` | |
| `6` | | |

Log in: `1` → `L002` → `alan123`

| Type | What appears | Say |
|---|---|---|
| `2` then `CS101` | **"Course CS101 is not assigned to you."** | "FR2.3 — a lecturer can only touch their own courses. Sixth failure path." |
| `6` | | |

---

## Act 3 — Administrator (2 min)

Log in: `1` → `A001` → `admin123`

| Type | What appears |
|---|---|
| `3` | full enrolment report, every course with its roll |
| `1` `a` → `Student` `S004` `Sunil Fernando` `pass123` | `[Success] Created Student S004` |
| `2` `a` → `LAB` `CS106` `Networks Lab` `2` `20` `L001` `6` `4` `600` `720` `Lab D` `CS101` | `[Success] Created course CS106` |
| `2` `b` → `CS106` → `Advanced Networks Lab` `35` `L002` | `[Success] Updated course CS106` |

Say: *"There are no setters on Course. An edit builds a new object and `Repository::add`
deletes the old one — and the prerequisite and time slot carry across."*

**Now the failure paths, back to back:**

| Type | What appears |
|---|---|
| `1` `a` → `Student` `S001` `Dup` `x` | **"A user with ID S001 already exists."** |
| `1` `a` → `Wizard` `S009` `Bad` `x` | **"Unknown role 'Wizard'."** |
| `2` `a` → `LEC` `X1` `Title` `abc` | **"That field needs a whole number."** |

Say: *"That last one was a bug our testing found. Typing letters used to leave `cin` in a
fail state and save a course with empty fields."*

| `4` then `2` | exits and saves |

---

## Act 4 — Persistence (1 min)

In the **second terminal**:

```powershell
Get-Content data\attendance.txt
Get-Content data\users.txt
```

Point at the `SESSION` line with its `RECORD` lines underneath, and `S004` in users.

Say: *"Saving happens immediately on every change, not at exit. And notice there's no
timetable file — timetables are derived."*

Restart, log in as `L001`, press `5` then `CS101`: **the attendance is still there.**

---

## Act 5 — Robustness (1 min, do this if time allows)

```powershell
Add-Content data\courses.txt "THIS IS A BROKEN LINE"
.\app.exe
```

```
[System Alert] Error: Corrupt data found in file data/courses.txt at line 6.
[System Alert] Saving is disabled until that line is fixed.
```

Exit with `2`:

```
[System Alert] Not saving: the data files were not fully loaded.
```

Then `Get-Content data\enrolments.txt` — **still intact**.

Say: *"Testing found this one. A corrupt line used to stop the load half way, and the save
on exit wrote the unloaded files out empty. Now a program that can't read its data isn't
allowed to write it."*

Clean up: `git checkout data/`

---

## The nine exception paths, in order

1. `AlreadyEnrolledException` — 2. `ClashException` — 3. `PrerequisiteNotMetException` —
4. `DuplicateAttendanceException` — 5. `NotEnrolledException` — 6. FR2.3 refusal —
7. duplicate user ID — 8. unknown role — 9. non-numeric input
(+ `CorruptDataException` in Act 5, and `CourseFullException` if you add the cut below)

---

## If you are short on time

Cut in this order: Act 5 → the live `console` capture in Act 2 → the admin *create* steps
(keep the three failure paths). **Never cut** the clash, the prerequisite, or the file replay.

## If you have spare time

Show `CourseFullException`: as admin, create a `LEC` course with **capacity 1** on day `6`
(Saturday, no clashes), then enrol two students — the second is refused with
*"Maximum capacity is 1."*

---

## Emphasis per member (the viva is individual)

Everyone runs the whole demo; lean on your own part when explaining.

- **Okith (M1)** — Person/Course hierarchies, the six enrolment rules in order, the
  exception tree. Linger on Act 1 and on `calculateCredits` in Act 2.
- **Nithika (M2)** — TimeSlot/Timetable and `operator&&`, the attendance core, the rule of
  five in `AttendanceSession`. Linger on the clash and on Act 2's capture.
- **Thishal (M3)** — `Repository<T>`, the file formats, the menu, `FileReplayCapture`.
  Linger on Act 4 and Act 5, and open `Repository.h` if asked.

---

## Questions you will probably get

**"Where's the polymorphism?"** — `user->showDashboard()` in `main.cpp`, one call, three
different menus, no `if` on a role string. And `calculateCredits()` giving 3 / 3 / 4 / 0.

**"Why pointers in the repository?"** — `map<string, T>` storing objects by value would
slice every `Student` down to a bare `Person` and destroy the subclass.

**"Why is `fromLine` static?"** — it must run *before* any object exists, and its address
is passed to `Repository::load` as a factory. A normal method would need an object first.

**"Why does `AttendanceSession` need all five special members?"** — it owns raw
`AttendanceRecord*`. Without deep copies, two sessions would delete the same records twice.

**"What's the singleton for?"** — the enrolment rules need users, courses and enrolments at
once. No domain class can see all three, so one reachable instance owns them.

**"How do you know it works?"** — the report's §5: save/load round-trips byte for byte,
nine exception paths, and three defects that testing found and we fixed.

---

## Resetting between members

```powershell
cd C:\Users\ASUS\unhinged
git checkout data/
```

Run that before every demo. Without it the second person starts with the first person's
enrolments and attendance, and the numbers won't match this script.
