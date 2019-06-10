using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace lw4
{
    class FileCurrencyReader
    {
        public FileCurrencyReader(string filePath)
        {
            _filePath = filePath;
        }

        public HashSet<string> ReadSet()
        {
            string fileContent;
            using (var reader = File.OpenText(_filePath))
            {
                fileContent = reader.ReadToEnd();
            }

            return new HashSet<string>(fileContent.Split(' '));
        }

        private string _filePath;
    }
}
