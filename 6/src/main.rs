use std::collections::HashMap;

fn main() {
    let input = include_str!("../data/input.txt");
    println!("Problem 1: {}", problem1(input));
    println!("Problem 2: {}", problem2(input));
}

fn map_sum(m: &HashMap<u32, u64>) -> u64 {
    let mut sum: u64 = 0;
    for (_, value) in m {
        sum += value;
    }
    sum
}

fn problem1(input: &str) -> u64 {
    let mut map: HashMap<u32, u64> = HashMap::new();
    for age in input.split(',').map(|n| n.trim().parse().unwrap()) {
        let count = map.entry(age).or_insert(0);
        *count += 1
    }

    for _day in 0..80 {
        let mut updated = HashMap::new();
        // Set 0 age fish to 6
        // And create new set of fish with age 8
        let new_fish = *map.entry(0).or_insert(0);
        updated.insert(8, new_fish);

        for age in 1..9 {
            let count = *map.entry(age).or_insert(0);
            // Decrement count of everything else as normal
            // Careful! We added some to entries 6 and 8 earlier
            updated.insert(age - 1, count);
        }
        // We overwrote entry 6 too quickly
        let last_count = *map.entry(7).or_insert(0);
        updated.insert(6, last_count + new_fish);
        map = updated;
    }

    map_sum(&map)
}

fn problem2(input: &str) -> u64 {
    let mut map: HashMap<u32, u64> = HashMap::new();
    for age in input.split(',').map(|n| n.trim().parse().unwrap()) {
        let count = map.entry(age).or_insert(0);
        *count += 1
    }

    for _day in 0..256 {
        let mut updated = HashMap::new();
        // Set 0 age fish to 6
        // And create new set of fish with age 8
        let new_fish = *map.entry(0).or_insert(0);
        updated.insert(8, new_fish);

        for age in 1..9 {
            let count = *map.entry(age).or_insert(0);
            // Decrement count of everything else as normal
            // Careful! We added some to entries 6 and 8 earlier
            updated.insert(age - 1, count);
        }
        // We overwrote entry 6 too quickly
        let last_count = *map.entry(7).or_insert(0);
        updated.insert(6, last_count + new_fish);
        map = updated;
    }

    map_sum(&map)
}

#[test]
fn test_problem1() {
    let input = include_str!("../data/sample.txt");
    assert_eq!(problem1(input), 5934);
}

#[test]
fn test_problem2() {
    let input = include_str!("../data/sample.txt");
    assert_eq!(problem2(input), 26984457539);
}
