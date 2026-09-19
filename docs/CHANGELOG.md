| Date | Change | Why | Approved by |
|------|--------|-----|-------------|
| 2026-09-17 | Repo skeleton created (include/, src/, Makefile) | Build baseline before implementation | M1, M2, M3 |
| 2026-09-19 | Exception header renamed SystemException.h -> Exceptions.h, source renamed to match | The include guard and every #include already said Exceptions.h; the file holds the whole hierarchy, not one class | M1 |
| 2026-09-19 | Enrolment.h and LectureCourse.h written (were empty placeholders) | Needed before M2 and M3 can use either class | M1 |
| 2026-09-19 | getCreditVal() body moved out of Course.h into Course.cpp | A function defined in a header is compiled into every .cpp that includes it, causing duplicate-symbol link errors | M1 |
| 2026-09-19 | Course keeps getTitle/getCapacity/getAssignedLectId/getPrereq/getEnrolledIds/getCreditVal/addPrereq, which the UML does not show | UML omits trivial accessors; keeping fields private with public getters is simpler than friend functions or protected fields, and Lecturer::viewEnrolmentList and Administrator::generateEnrolmentReport need the enrolment list | M1 |
| 2026-09-19 | Course::slots (vector<TimeSlot>) declared but commented out | TimeSlot is M2's class; the vector needs the complete type before Course can be constructed. Uncomment when M2 pushes TimeSlot.h | M1 |
| 2026-09-19 | Student's 4th constructor parameter accepted but unused | StudentCard is M2's class; the parameter stays in the signature so no call site changes when M2 lands | M1 |
