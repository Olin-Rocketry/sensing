import struct, csv

def binary_to_csv(binary_file_location: str, csv_file_location: str, num_bytes: int, format: str) -> None:
    binary_bytes = None
    with open(binary_file_location, "rb") as data_file:
        binary_bytes = data_file.read()
    converted_bytes = [0] * round(len(binary_bytes) / num_bytes)
    for i in range(0, len(binary_bytes), num_bytes):
        converted_bytes[round(i / num_bytes)] = struct.unpack(format, binary_bytes[i: i + num_bytes])

    with open(csv_file_location, 'w', newline='') as write_file:
        csv_out = csv.writer(write_file)
        csv_out.writerow(["Time since launch (ms)", "X Acceleration (m/s)", "Y Acceleration (m/s)", "Z Acceleration (m/s)", "X Orientation", "Y Orientation", "Z Orientation", "Global Z Accel", "Altitude (m)", "Latitude", "Longitude", "Phase", "Kalman Altitude", "Global Z Vel","Kalman Global Z Accel"])
        for row in converted_bytes:
            if row[0] != 0:
                csv_out.writerow(row)
