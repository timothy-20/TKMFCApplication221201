#pragma once

// 23.01.04 - baekjoon 1123
#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>

struct TKSong
{
	uint32_t genre;
	uint32_t playTime;

	TKSong() = default;
	TKSong(uint32_t genre, uint32_t playTime) :
		genre(genre),
		playTime(playTime)
	{}
	TKSong(const TKSong& other) :
		genre(0),
		playTime(0)
	{
		*this = other;
	}
	TKSong(TKSong&& other) noexcept :
		genre(0),
		playTime(0)
	{
		*this = std::move(other);
	}
	TKSong& operator=(const TKSong& other)
	{
		if (this != &other)
		{
			this->genre = other.genre;
			this->playTime = other.playTime;
		}

		return *this;
	}
	TKSong& operator=(TKSong&& other) noexcept
	{
		if (this != &other)
		{
			this->genre = std::exchange(other.genre, 0);
			this->playTime = std::exchange(other.playTime, 0);
		}

		return *this;
	}
};

struct TKPlayList
{
private:
	std::vector<TKSong> m_songs;
	uint32_t m_totalPlayTime;

public:
	TKPlayList() :
		m_songs(std::vector<TKSong>()),
		m_totalPlayTime(0)
	{}
	TKPlayList(std::vector<TKSong> songs) : TKPlayList()
	{
		for (auto iterator(songs.cbegin()); iterator != songs.cend(); iterator++)
			this->AddSong(*iterator);
	}

	// Utils
	void AddSong(const TKSong& song)
	{
		this->m_songs.push_back(song);
		this->m_totalPlayTime += song.playTime;
	}
	std::vector<TKSong> GetSongs() const
	{
		return this->m_songs;
	}
	uint32_t GetTotalPlayTime() const
	{
		return this->m_totalPlayTime;
	}
};

class TKPlayListShuffler
{
private:
	std::vector<TKSong> m_songs;
	std::vector<TKPlayList> m_patterns;
	std::vector<std::vector<uint32_t>> m_adjacentList;


	void CreatePlayListPatternsPermutation(int s = 0)
	{
		size_t size(this->m_songs.size());

		if (s == size)
		{
			this->m_patterns.push_back(this->m_songs);
			return;
		}

		for (int i(s); i < size; i++)
		{
			std::swap(this->m_songs[s], this->m_songs[i]);
			this->CreatePlayListPatternsPermutation(s + 1);
			std::swap(this->m_songs[s], this->m_songs[i]);
		}
	}

	void CreatePlayListPatternsCombination(std::vector<TKSong> combination, int index = 0)
	{
		if (combination.size() == index)
		{
			this->m_patterns.push_back(combination);
			return;
		}

		for (int i(0); i < this->m_songs.size(); i++)
		{
			combination[index] = this->m_songs[i];

			this->CreatePlayListPatternsCombination(combination, index + 1);
		}
	}

	bool CheckPlayListAvailablePattern(const TKPlayList& playList)
	{
		bool result(false);
		auto songs(playList.GetSongs());

		if (songs.size() == 1)
			return true;

		for (int i(0); i < songs.size() - 1; i++)
		{
			std::vector<uint32_t> vector(this->m_adjacentList[songs[i].genre]);
			result = (std::count(vector.cbegin(), vector.cend(), songs[i + 1].genre) == 1);
		}

		return result;
	}

public:
	TKPlayListShuffler(const TKPlayList& totalPlayList) :
		m_songs(totalPlayList.GetSongs()),
		m_patterns(std::vector<TKPlayList>())
	{
		std::set<uint32_t> temp;

		for (auto song : this->m_songs)
		{
			if (temp.find(song.genre) == temp.end())
				temp.insert(song.genre);
		}

		this->m_adjacentList = std::vector<std::vector<uint32_t>>(temp.size());

		for (int i(0); i < this->m_adjacentList.size(); i++)
		{
			std::string adjacencyVector;

			std::cin >> adjacencyVector;

			for (int j(0); adjacencyVector[j] != '\0'; j++)
			{
				if (adjacencyVector[j] == 'Y')
					this->m_adjacentList[i].push_back(j);
			}
		}
	}

	// Utils
	std::vector<TKPlayList> CreatePatternWithTotalPlayTime(int totalPlayTime)
	{
		for (int i(1); i < this->m_songs.size(); i++)
			this->CreatePlayListPatternsCombination(std::vector<TKSong>(i));

		this->m_patterns.erase(std::remove_if(this->m_patterns.begin(), this->m_patterns.end(), [this, totalPlayTime](TKPlayList playList) -> bool
			{
				if (playList.GetTotalPlayTime() == totalPlayTime)
				return !this->CheckPlayListAvailablePattern(playList);

		return true;

			}), this->m_patterns.end());

		std::vector<TKPlayList> specificPattern(this->m_patterns);

		this->m_patterns.clear();

		return specificPattern;
	}
};

void Run()
{
	TKPlayList playList;
	int songsNumber;

	std::cin >> songsNumber;

	for (int i(0); i < songsNumber; i++)
	{
		int n1, n2;

		std::cin >> n1 >> n2;
		playList.AddSong(TKSong(n1, n2));
	}

	int genreNumber;

	std::cin >> genreNumber;

	TKPlayListShuffler shuffler(playList);
	std::vector<TKPlayList> result;
	int a, b;

	std::cin >> a >> b;

	for (; a <= b; a++)
	{
		auto patterns(shuffler.CreatePatternWithTotalPlayTime(a));

		result.insert(result.end(), patterns.begin(), patterns.end());
	}

	std::cout << result.size() << std::endl;
}