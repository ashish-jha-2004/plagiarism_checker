### Test Case

**Consider the following two submissions:**

- **Submission 1:** `{1, 2, 3, 4, 5}`
- **Submission 2:** `{1, 6, 7, 8, 5}`

---

### Calculation

#### Levenshtein Distance:
- **Changes:**
  - Replace `2` with `6` → 1 edit.
  - Replace `3` with `7` → 1 edit.
  - Replace `4` with `8` → 1 edit.
- **Total edits = 3.**
- **Levenshtein distance = 3.**

#### Longest Common Subsequence (LCS):
- The LCS is `{1, 5}`.
- **Length of LCS = 2.**

---

### Criteria Check

- **Levenshtein distance** passes because the number of edits (`3`) is less than `20%` of the larger submission size (`5`), i.e., `0.2 * 5 = 1`.
- **LCS** fails because its length (`2`) is not greater than or equal to `60%` of the larger submission size (`5`), i.e., `0.6 * 5 = 3`.
