# Makefile for System Programming 2 - Ex1

# מהדר C++ (ודא שהנתיב תקין אם g++ לא ב-PATH)
CXX := g++
# במקרה שלך, ייתכן שתצטרך את הנתיב המלא:
# CXX := C:/Strawberry/c/bin/g++.exe

# דגלי מהדר (Compiler Flags)
# -std=c++17 : שימוש בתקן C++17
# -Wall       : הפעלת כל האזהרות
# -Werror     : (אופציונלי) התייחסות לאזהרות כשגיאות
# -g          : הוספת מידע לדיבוג
CXXFLAGS := -std=c++17 -Wall -g
# CXXFLAGS += -Werror # אפשר להוסיף אם רוצים להיות קפדניים

# דגלי קדם-מעבד (Preprocessor Flags), בעיקר לנתיבי include
# -I. : הוספת התיקייה הנוכחית לנתיבי החיפוש (עבור doctest.h)
CPPFLAGS := -I.

# דגלי מקשר (Linker Flags) - לא נחוצים כרגע
# LDFLAGS :=

# קבצי המקור המשותפים (ה"ספרייה" שלנו)
SRCS_LIB := Graph.cpp Algorithms.cpp DataStructures.cpp
# קובץ המקור הראשי
SRC_MAIN := main.cpp
# קובץ המקור של הבדיקות
SRC_TEST := tests.cpp

# קבצי אובייקט (Object files)
OBJS_LIB := $(SRCS_LIB:.cpp=.o) # Graph.o Algorithms.o DataStructures.o
OBJ_MAIN := $(SRC_MAIN:.cpp=.o) # main.o
OBJ_TEST := $(SRC_TEST:.cpp=.o) # tests.o

# שמות קבצי ההרצה
EXEC_MAIN := main.exe
EXEC_TEST := runTests.exe

# פקודת מחיקה (rm -f אמור לעבוד גם ב-MinGW, -f מתעלם משגיאות אם קובץ לא קיים)
# אם זה לא עובד, אפשר לנסות: RM := del /Q
RM := rm -f


# --- מטרות (Targets) ---

# המטרה הראשונה היא ברירת המחדל (make בלי ארגומנטים יבצע את זה)
all: $(EXEC_MAIN) $(EXEC_TEST)

# המטרה 'Main' כפי שנדרש במטלה
Main: $(EXEC_MAIN)
	@echo "Running main program..."
	.\$(EXEC_MAIN)

# בניית קובץ ההרצה הראשי
$(EXEC_MAIN): $(OBJ_MAIN) $(OBJS_LIB)
	@echo "Linking main executable..."
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# המטרה 'test' כפי שנדרש במטלה
test: $(EXEC_TEST)
	@echo "Running tests..."
	.\$(EXEC_TEST)

# בניית קובץ ההרצה של הבדיקות
$(EXEC_TEST): $(OBJ_TEST) $(OBJS_LIB)
	@echo "Linking test executable..."
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# המטרה 'valgrind' כפי שנדרש במטלה
# !!! חשוב: Valgrind הוא כלי של לינוקס. מטרה זו תעבוד רק אם אתה מריץ make
# !!! בסביבת לינוקס (כמו WSL) שבה valgrind מותקן, וקימפלת את הקוד *עבור לינוקס*.
# !!! היא לא תעבוד ישירות על Windows.
valgrind: $(EXEC_MAIN)
	@echo "Running valgrind (requires Linux environment and valgrind installed)..."
	valgrind --leak-check=full .\$(EXEC_MAIN)

# כלל גנרי לבניית קבצי אובייקט (.o) מקבצי מקור (.cpp)
%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# המטרה 'clean' כפי שנדרש במטלה
clean:
	@echo "Cleaning up generated files..."
	$(RM) $(EXEC_MAIN) $(EXEC_TEST) *.o
	@echo "Cleanup finished."


# הגדרת מטרות שאינן קבצים (כדי למנוע בלבול אם יש קובץ בשם זהה)
.PHONY: all Main test valgrind clean