# days

## Blob

The blob for the calendar can be found in the [data](/data/calendar.bin) directory.

### Format

> [!NOTE]
>
> There are no separators, and the `0`'s on the left are just to illustrate the size of each field.
> Entries repeat after the header until the end of the file.

```txt
HEADER (2 BYTES)

0000 0000 - Year of compilation in little endian

ENTRIES (2 BYTES)

0000      - First 4 bits for years after compilation
0000      - Next 4 bits for month

00000     - First 5 bits for day
000       - Next 3 bits for asb_day
```

```c
struct Day {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t asb_day;
};
```

### Generating

If need be, you can regenerate the blob by running the [rust program](calendar).
You'll have to do this about every year, since that's when the ASB calendar is updated

To use it, just make sure you save the community calendar as `calendar.ics` in the root directory of the rust program.
(in other words, same level as `Cargo.toml`.)

Then, just run `cargo run`. The new blob will be saved as `output.bin` and `output.h` for the embedded C code.
