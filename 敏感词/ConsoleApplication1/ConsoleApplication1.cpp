#include<locale>//本地化设定
#include<iostream>
#include<string>
#include<fstream>
using namespace std;
int main()
{
	string wos, ors, ans;
	cin >> wos;
	cin >> ors; 
	cin>> ans;
	locale china("zh_CN.UTF-8");//设置本地是中国，这样才能在文件中都入读出中文，而不乱码
	//下面是打开org
	wifstream infile2;
	infile2.open(ors);//用来打开文档
	if (!infile2) { cout << "org fail!" << endl; }
	infile2.imbue(china);//读入的是中文
	wstring org1;
	//wstring 类是两个字节放一个字，如“汉”可以放org1[0]中而用string a需要a[0]a[1]两个储存一个汉字
	int line1 = 0, total = 0;//line1是储存第几行,total是储存搜索出来多少个敏感词
	ofstream outfile;//普通的文件输出
	outfile.open(ans);
	wofstream woutfile;//用来输出wstring
	woutfile.open(ans);
	woutfile.imbue(china);//输出中文
	woutfile << "                            " << endl;//在输出文件的第一行输出一堆空格，=
	while (getline(infile2, org1))//用来读入一行待检测文件
	{
		line1++;//待检测文件行数加一
		for (int i = 0; i <= org1.size(); i++)
		{//wcout <<org1.size()<<' ' << words1.size() << endl;
			wifstream infile1;
			infile1.open(wos);//用来打开敏感词
			if (!infile1) { cout << "words fail!" << endl; }
			wcout.imbue(china);
			infile1.imbue(china);//设置中国本地
			wstring words1;//用来存放一行的敏感词
			while (getline(infile1, words1))//读入一行敏感词
			{
				if (((words1[0] >= 'a' && words1[0] <= 'z') || (words1[0] >= 'A' && words1[0] <= 'Z'))
					&& ((org1[i] == words1[0]) || (org1[i] == words1[0] + 32) || (org1[i] == words1[0] - 32)))
				{//如果是英文
					int x = 1, sum = 0;//x用来驱动words[x]
					for (int j = 1; (i + j <= org1.size()); j++)
					{//如果找到了对应的第一个字符
						sum++;
						if (sum > 21)
						{//根据题意两个敏感词之间的字符超过sum个，如果超过就不检测
							break;
						}
						if ((org1[i + j] == words1[x] || org1[i + j] == words1[x] - 32 || org1[i + j] == words1[x] + 32) && x <= words1.size())
						{//x++是说明这个敏感词words1[x]和待检测文件orgs1的字符对应上，+1检测下一个
							x++; sum = 0;
						}
						else if ((org1[i + j] >= 'a' && org1[i + j] <= 'z') || (org1[i + j] >= 'A' && org1[i + j] <= 'Z'))
						{//如果英文之间有其他英文则不属于敏感词，退出循环
							break;//如fuack，就不是敏感词，检测到a之后就不用判断了
						}
						if (x == (words1.size()))
						{//如果检测出是敏感词
							total++;//检测到的敏感词+1
							woutfile << "Line" << line1 << ":<";
							woutfile << words1;
							woutfile << '>';//输出："Line x:<敏感词>”
							for (int k = i; k <= i + j; k++)
							{//输出待检测文档的敏感词
								woutfile << org1[k];
							}
							woutfile << endl;
							break;
						}
					}

				}
				else if (org1[i] == words1[0])
				{//如果是中文
					int x = 1, sum = 0;
					for (int j = 1; (i + j <= org1.size()); j++)
					{//如果找到了对应的第一个汉字
						sum++;
						if (sum > 21)
						{//根据题意两个敏感词之间的字符超过sum个，如果超过就不检测
							break;
						}
						if (org1[i + j] == words1[x] && x <= words1.size())
						{//如果找到了就往下判断，同时检测下一个敏感字，sum重新归零
							x++;
							sum = 0;
						}
						else if ((org1[i + j] >= 255) || (org1[i + j] >= '0' && org1[i + j] <= '9')
							|| (org1[i + j] >= 'a' && org1[i + j] <= 'z')
							|| (org1[i + j] >= 'A' && org1[i + j] <= 'Z'))
						{//敏感词汉字之间有字母、数字、汉字就不再是敏感词
							break;
						}
						if (x == (words1.size()))
						{//找到敏感词输出
							total++;
							woutfile << "Line" << line1 << ":<";
							woutfile << words1;
							woutfile << '>';
							for (int k = i; k <= i + j; k++)
							{
								woutfile << org1[k];
							}
							woutfile << endl;
							break;
						}
					}
				}
			}
			infile1.close();//关闭文件
		}

	}
	infile2.close();//关闭文件
	outfile << "Total:" << total;//输出检测出来的敏感词总数，会覆盖掉文件前面的空格
	outfile.close();//关闭文件
	woutfile.close();//关闭文件
	return 0;
}
/*
d:words.txt
d:org.txt
d:ans.txt
*/