use chrono::{Datelike, NaiveDate, Utc};
use std::{
    fs::{self, OpenOptions, read},
    io::Write,
};

use icalendar::{Calendar, Component, Event};

// Only exist for clarity.
type U3 = u8;
type U4 = u8;
type U5 = u8;

fn combine53(first: U5, second: U3) -> u8 {
    (first << 3) | second
}

fn combine44(first: U4, second: U4) -> u8 {
    (first << 4) | second
}

fn split44(byte: u8) -> (u8, u8) {
    let high = byte >> 4;
    let low = byte & 0x0F;
    (high, low)
}

fn split53(byte: u8) -> (u8, u8) {
    let high = byte >> 3;
    let low = byte & 0b0000_0111;
    (high, low)
}

fn entry(today: NaiveDate, date: NaiveDate, asb_day: U3) -> Option<[u8; 2]> {
    Some([
        combine44(date.years_since(today)? as U4, date.month0() as U4 + 1),
        combine53(date.day() as u8, asb_day),
    ])
}

fn main() {
    let contents = include_str!("../calendar.ics");
    let calendar: Calendar = contents.parse().unwrap();

    let mut output = OpenOptions::new()
        .create(true)
        .write(true)
        .append(false)
        .open("output.bin")
        .unwrap();

    output.set_len(0).unwrap();

    fn date(event: &Event) -> NaiveDate {
        event.get_start().unwrap().date_naive()
    }

    let mut events: Vec<&Event> = calendar
        .components
        .iter()
        .filter_map(|x| x.as_event())
        .collect();
    events.sort_by(|a, b| date(a).cmp(&date(b)));

    let cutoff = events.iter().position(|x| date(x).year() == 2025).unwrap();
    events.drain(0..cutoff);

    let today = Utc::now().naive_utc().date();
    let year = today.year() as u16;

    output.write(&[year as u8, (year >> 8) as u8]).unwrap();

    for event in events.into_iter() {
        let asb_day: U3 = if let Some(day) = event.get_summary().unwrap().strip_prefix("Day ") {
            day.parse::<U3>().unwrap()
        } else {
            continue;
        };

        let date = event.get_start().unwrap().date_naive();
        if let Some(entry) = entry(today, date, asb_day) {
            output.write(&entry).unwrap();
        }
    }

    output.flush().unwrap();

    let bytes: Vec<u8> = read("./output.bin").unwrap();
    let formatted: String = bytes.iter().map(|x| format!("{x:#04x}, ")).collect();

    let c = format!("unsigned char data[{}] = {{ {formatted}}};", bytes.len());
    fs::write("./output.h", c).unwrap();

    let compyear = u16::from_le_bytes([bytes[0], bytes[1]]);
    eprintln!("COMPILE YEAR: {}", compyear);

    // Decoding
    for data in bytes.chunks(2).skip(1) {
        eprintln!("{:08b} {:08b}", data[0], data[1]);
        let (year_diff, month) = split44(data[0]);
        let (day, asb_day) = split53(data[1]);

        let year = compyear as u16 + year_diff as u16;
        let month = month;

        eprintln!("ENTRY: {}-{}-{}: {asb_day}", year, month, day);
    }
}
