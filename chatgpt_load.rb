# File: combine_files.rb

# Array of relative file paths
file_paths = [
  'main.c',
  'gem.h',
  'gem.c',
  'constants.h',
]

# Output file path
output_file_path = 'combined_output.txt'

# Open the output file for writing
File.open(output_file_path, 'w') do |output_file|
  file_paths.each do |file_path|
    # Write the header with the file path
    output_file.puts "===== File: #{file_path} ====="

    # Read the contents of the current file and write to the output file
    File.open(file_path, 'r') do |input_file|
      input_file.each_line do |line|
        output_file.puts line
      end
    end

    # Add a blank line between files for better readability
    output_file.puts "\n"
  end
end

puts "All files have been combined into #{output_file_path}"
