export type AllComments = Record<number, Comments>;

export type Comments = Comment[];

export type Comment = {
  author: string;
  img: string;
  comment_content: string;
};

export const all_comments: AllComments = {
  1: [
    {
      author: "Vlad Viking",
      img: "https://www.stickees.com/files/funny/funny-people/4098-viking-sticker.png",
      comment_content:
        "I've been using you guys forever, will you stick around?",
    },
  ],
  2: [
    {
      author: "Viktor Viking",
      img: "https://images.vexels.com/media/users/3/126897/isolated/lists/b88fb83462d19cf27556b4c44e8e807a-viking-warrior-avatar.png",
      comment_content:
        "Dude, YOU LOST MY AXE. Stop apologizing and get it back",
    },
  ],
};
