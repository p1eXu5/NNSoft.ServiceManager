
namespace NNSoft.ServiceManager.Adapter
{
    public readonly struct Service
    {
        public Service( int pid, string name, string description, Statuses status, string group, string imagePath )
        {
            Pid = pid;
            Name = name;
            Description = description;
            Status = status;
            Group = group;
            ImagePath = imagePath;
        }


        public int Pid { get; }
        public string Name { get; }
        public string Description { get; }
        public Statuses Status { get; }
        public string Group { get; }

        public string ImagePath { get; }
    }
}
