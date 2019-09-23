import argparse
from difflib import SequenceMatcher
import os


parser = argparse.ArgumentParser()

parser.add_argument('proto_path', help='proto location')
parser.add_argument('proto_include_path', help='proto include path')

if __name__ == '__main__':
    args = parser.parse_args()
    # print(args.proto_path)
    # print(args.proto_include_path)

    match = SequenceMatcher(None, args.proto_path, args.proto_include_path).\
        find_longest_match(
            0, len(args.proto_path),
            0, len(args.proto_include_path)
        )
    if match.size == 0:
        canonical_path = os.path.basename(args.proto_path)
    else:
        canonical_path = args.proto_path[match.a + match.size:]
    if canonical_path.startswith('/'):
        canonical_path = canonical_path[1:]
    print(canonical_path)
